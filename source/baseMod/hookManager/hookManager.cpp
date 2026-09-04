#include "hookManager.h"
#include <atomic>
#include <d3d9.h>
#include <vector>
#include "baseMod_p.h"
#include "managedHook.h"
#include "patch.h"
#include "offsets.h"


static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_PeekMessageInfo*> _afterPeekMessageCallbacks;
static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_GameUpdateInfo*> _beforeGameUpdateCallbacks;
static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_GameUpdateInfo*> _afterGameUpdateCallbacks;
static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_DrawInfo*> _beforeEndSceneCallbacks;
static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_DrawInfo*> _beforePresentCallbacks;
static ManagedHookCallbacks<void, const BaseMod_HookContext*, const BaseMod_SaveGameInfo*> _afterSaveGameCallbacks;

static std::atomic<BaseMod_HookId> _nextId{1};

template <typename Hook, typename Info>
inline BaseMod_HookId RegisterHook(
    Hook hook,
    void* userData,
    ManagedHookCallbacks<void, const BaseMod_HookContext*, Info>& callbacks
) {
    if (!hook) return 0;

    BaseMod_HookId id = _nextId.fetch_add(1, std::memory_order_relaxed);
    callbacks.registerHook(id, hook, userData);

    return id;
}
BaseMod_HookId BASEMOD_CALL AfterPeekMessage(BaseMod_PeekMessageHook hook, void* userData) {
    return RegisterHook(hook, userData, _afterPeekMessageCallbacks);
}
BaseMod_HookId BASEMOD_CALL BeforeGameUpdate(BaseMod_GameUpdateHook hook, void* userData) {
    return RegisterHook(hook, userData, _beforeGameUpdateCallbacks);
}
BaseMod_HookId BASEMOD_CALL AfterGameUpdate(BaseMod_GameUpdateHook hook, void* userData) {
    return RegisterHook(hook, userData, _afterGameUpdateCallbacks);
}
BaseMod_HookId BASEMOD_CALL BeforeEndScene(BaseMod_DrawHook hook, void* userData) {
    return RegisterHook(hook, userData, _beforeEndSceneCallbacks);
}
BaseMod_HookId BASEMOD_CALL BeforePresent(BaseMod_DrawHook hook, void* userData) {
    return RegisterHook(hook, userData, _beforePresentCallbacks);
}
BaseMod_HookId BASEMOD_CALL AfterSaveGame(BaseMod_SaveGameHook hook, void* userData) {
    return RegisterHook(hook, userData, _afterSaveGameCallbacks);
}

// This could stand to be smarter
uint32_t BASEMOD_CALL RemoveHook(BaseMod_HookId id) {
    bool success = _afterPeekMessageCallbacks.unregisterHook(id);
    if (success) return success;
    success = _beforeGameUpdateCallbacks.unregisterHook(id);
    if (success) return success;
    success = _afterGameUpdateCallbacks.unregisterHook(id);
    if (success) return success;
    success = _beforeEndSceneCallbacks.unregisterHook(id);
    if (success) return success;
    success = _beforePresentCallbacks.unregisterHook(id);

    return success;
}


BOOL WINAPI PeekMessageWWrapper(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
    BOOL result = PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

    const BaseMod_HookContext ctx = { HookType::PEEK_MESSAGE };
    const BaseMod_PeekMessageArgs args = {
        (MSG*)lpMsg, (void*)hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg
    };
    const BaseMod_PeekMessageInfo info = {args, result};

    _afterPeekMessageCallbacks.invokeAll(&ctx, &info);

    return result;
}

static void* peekMessageHookOriginalBytes;
static void* peekMessageRelJmpOffsetOriginalByte;
inline void InstallPeekMessageHook() {
    void* injectAddress = getBaseAddress() + offsets::PEEK_MESSAGE_FUNCTION_POINTER;
    void* hookAddress = reinterpret_cast<void*>(PeekMessageWWrapper);
    
    Patch(injectAddress, &hookAddress, sizeof(hookAddress), &peekMessageHookOriginalBytes);

    // Install pointer update asm
    char asmByte = 0xB8;

    injectAddress = getBaseAddress() + offsets::MESSAGE_LOOP_REL_JMP_OFFSET_BYTE_ADDR;
    Patch(injectAddress, &asmByte, sizeof(asmByte), &peekMessageRelJmpOffsetOriginalByte);
}

static void(__stdcall *const NativeCommonSimUpdate)() =
    reinterpret_cast<void(__stdcall *)()>(getBaseAddress() + offsets::COMMON_SIM_UPDATE_FUNC);
void __stdcall CommonSimUpdateWrapper() {
    BaseMod_HookContext ctx = { HookType::UPDATE };
    // TODO: is there any info that should be passed?
    _beforeGameUpdateCallbacks.invokeAll(&ctx, nullptr);

    NativeCommonSimUpdate();

    _afterGameUpdateCallbacks.invokeAll(&ctx, nullptr);
}
static void* updateGameStateOriginalBytes;
inline void InstallGameUpdateHook() {
    void* injectAddress = getBaseAddress() + offsets::COMMON_SIM_UPDATE_FUNCTION_CALL + 1;
    void* hookAddress = reinterpret_cast<void*>(CommonSimUpdateWrapper);

    Patch_RelativeJump(
        injectAddress,
        hookAddress,
        &updateGameStateOriginalBytes
    );
}

static void (__stdcall *const NativeSetGraphicsContext)() =
    reinterpret_cast<void(__stdcall *)()>(getBaseAddress() + offsets::SET_GRAPHICS_CONTEXT_FUNC);
static IDirect3DDevice9* _nativeDevice = *reinterpret_cast<IDirect3DDevice9**>(getBaseAddress() + offsets::DIRECT3D9_DEVICE);
void __stdcall SetGraphicsContextWrapper() {
    NativeSetGraphicsContext();

    BaseMod_HookContext ctx = { HookType::DRAW };
    BaseMod_DrawInfo info = { _nativeDevice };
    _beforeEndSceneCallbacks.invokeAll(&ctx, &info);
}

static void* setGraphicsContextCallOriginalBytes;
inline void InstallEndSceneHook() {
    // This inject address is a CALL instruction of a function called right before EndScene.
    //  this hook will replace the function called here with a wrapper function (`SetGraphicsContextWrapper`).
    //  +1 to skip over the opcode so we have the address of the operand.
    void* injectAddress = getBaseAddress() + offsets::SET_GRAPHICS_CONTEXT_CALL + 1;
    void* hookAddress = reinterpret_cast<void*>(SetGraphicsContextWrapper);

    Patch_RelativeJump(
        injectAddress,
        hookAddress,
        &updateGameStateOriginalBytes
    );
}

HRESULT __stdcall PresentWrapper(IDirect3DDevice9* dummyDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
    static IDirect3DDevice9* nativeDevice = *reinterpret_cast<IDirect3DDevice9**>(getBaseAddress() + offsets::DIRECT3D9_DEVICE);
    static BaseMod_HookContext ctx = { HookType::DRAW };
    static BaseMod_DrawInfo info = { nativeDevice };

    _beforePresentCallbacks.invokeAll(&ctx, &info);
    
    return nativeDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
// This is a fake IDirect3DDevice9 struct that will be inserted before the Present call.
//  In the code where this will be inserted, only the Present function is referenced.
typedef HRESULT (__stdcall *D3D9Present)(IDirect3DDevice9* device, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
struct DummyD3D9DeviceVTable {
    std::byte _padding[0x44];
    D3D9Present Present;
};
struct DummyD3D9Device {
    DummyD3D9DeviceVTable* VTable;
};
static IDirect3DDevice9* _originalDevicePtr;
inline void InstallPresentHook() {
    static DummyD3D9DeviceVTable _dummyVTable { Present: PresentWrapper };
    static DummyD3D9Device _dummyDevice = { VTable: &_dummyVTable };
    static DummyD3D9Device* _pDummyDevice = &_dummyDevice;
    static DummyD3D9Device** _payload = &_pDummyDevice;

    // This inject address is an instruction that loads the d3d9 device.
    //  +1 to skip over the opcode so we have the address of the operand we want to overwrite.
    void* injectAddress = getBaseAddress() + offsets::BEFORE_PRESENT_DEVICE_REFERENCE + 1;

    // Replacing the absolute address of the d3d9 device with the address of our dummy struct.
    Patch(injectAddress, &_payload, sizeof(&_payload), &_originalDevicePtr);
}

static void __stdcall (* const NativeSaveGame)() =
    reinterpret_cast<void __stdcall (*)()>(getBaseAddress() + offsets::SAVE_GAME_DATA_FN);
void __stdcall SaveGameDataWrapper() {
    // The original function is called with the value of ESI as it's input parameter.
    //  This asm block will copy it to a C variable while maintaining
    //  the register value when invoking the original function.
    void* param;
    asm(
        "movl %%esi, %[aParam]\n\t"
        "call *%[fn]"
        : [aParam] "=m" (param)
        : [fn] "g" (NativeSaveGame)
        : "cc"
    );

    static BaseMod_HookContext ctx = { HookType::DRAW };
    static BaseMod_SaveGameInfo info = { param };
    _afterSaveGameCallbacks.invokeAll(&ctx, &info);
}

inline void InstallSaveGameHook() {
    void* injectAddress = getBaseAddress() + offsets::SAVE_GAME_DATA_FN_CALL + 1;
    void* hookAddress = reinterpret_cast<void*>(SaveGameDataWrapper);

    Patch_RelativeJump(
        injectAddress,
        hookAddress,
        nullptr
    );
}

// TEMP
#include <iostream>

static const void* NativeGetResourcePath = getBaseAddress() + 0x365440;
char* __stdcall ResourceLookUpWrapper(void* redirect_map, void* reserve_1) {
    // Save register params
    void* reserve_0;
    char* path;
    asm(
        "" // no asm
        : "=c" (reserve_0)
        , "=d" (path)
    );

    std::cout << "[DEBUG] Resource Lookup: " << path << std::endl;

    // Invoke original function
    char* output;
    asm(
        "push %[aReserve_1]\n\t"
        "push %[aMap]\n\t"
        "call *%[fn]"
        : "=g" (output)
        : [fn] "g" (NativeGetResourcePath)
        , [aMap] "g" (redirect_map)
        , [aReserve_1] "g" (reserve_1)
        , "c" (reserve_0)
        , "d" (path)
        : "cc", "memory"
    );

    std::cout << "[DEBUG] Output: " << (output ? output : "") << std::endl;

    // forward original function output
    return output;
}

inline void InstallResourceLookupHook() {
    constexpr intptr_t fnCallAddr = 0x1139B6;
    void* injectAddress = getBaseAddress() + fnCallAddr + 1;
    void* hookAddress = reinterpret_cast<void*>(ResourceLookUpWrapper);

    Patch_RelativeJump(
        injectAddress,
        hookAddress,
        nullptr
    );
}

void InstallHooks() {
    InstallPeekMessageHook();
    InstallGameUpdateHook();
    InstallEndSceneHook();
    InstallPresentHook();
    InstallSaveGameHook();
    InstallResourceLookupHook();
}

static const BaseMod_HookApi _hookApi = {
    sizeof(BaseMod_HookApi),
    BASEMOD_API_VERSION_NUM,
    
    RemoveHook,
    AfterPeekMessage,
    BeforeGameUpdate,
    AfterGameUpdate,
    BeforeEndScene,
    BeforePresent,
    AfterSaveGame,
};
const BaseMod_HookApi* GetHookApi() { return &_hookApi; }
