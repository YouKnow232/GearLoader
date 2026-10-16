#include "gearLoader/gearLoader.hpp"
#include "baseMod/baseMod.hpp"
#include <d3d9.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <winuser.h>
#include "testGraphics.h"

static int displayData = 0;
static int textZPos = 5;
static BaseMod::Api* bmApi;
static int logFrameTime = 0;
static int enableZTest = 0;
static int zTestPos = 255;

struct TestContext {
    GearLoader::Api* glApi;
    BaseMod::Api* bmApi;
};
static TestContext _testCtx;

inline std::string ToHexString(int value) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(sizeof(int)*2) << std::uppercase << std::hex << value;
    return ss.str();
}
inline void logErr(const char* str, int err) {
    std::cout << "[baseModTester][ERR] " << str << ": " << err << std::endl;
}

void __stdcall testPeekMessageHook(
    TestContext* uCtx,
    const BaseMod_HookContext* hCtx,
    const BaseMod_PeekMessageInfo* info
) {
    static bool tested = false;

    if (!tested) {
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "peekMessage hook called");
        tested = true;
    }

    MSG* msg = reinterpret_cast<MSG*>(info->args.lpMsg);
    if (info->success && msg->message == WM_KEYDOWN &&
        msg->wParam == VK_F1
    ) {
        std::cout << "[baseModTester] F1 press detected via AfterPeekMessage" << std::endl;
    }
}
void __stdcall testBeforeGameUpdateHook(
    TestContext* uCtx,
    const BaseMod_HookContext* hCtx,
    const BaseMod_GameUpdateInfo* info
) {
    static bool tested = false;

    if (!tested) {
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "BeforeGameUpdate hook called");
        tested = true;
    }
}
void displayApiTest1(BaseMod::Api& api) {
        constexpr int xPosLeftCol = 10;
        constexpr int xPosRightCol = 340;
        auto& fn = api.NativeFunctions;
        auto& data = api.GameData;
        auto p1 = api.GameData.GetPlayer(0);
        auto p2 = api.GameData.GetPlayer(1);
        auto cam = api.GameData.GetCamera();
        float zPos = static_cast<float>(textZPos);

        fn.RenderCockpitFontText("ACT ID: " + std::to_string(p1.actId()), xPosLeftCol, 10, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT TIMER: " + std::to_string(p1.actTimer()), xPosLeftCol, 30, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("POS: (" + std::to_string(p1.position().x) + "," + std::to_string(p1.position().y) + ")", 10, 50, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("ACT ID: " + std::to_string(p2.actId()), xPosRightCol, 10, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT TIMER: " + std::to_string(p2.actTimer()), xPosRightCol, 30, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("POS: (" + std::to_string(p2.position().x) + "," + std::to_string(p1.position().y) + ")", xPosRightCol, 50, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("HEALTH: " + std::to_string(p1.health()), xPosLeftCol, 70, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("TENSION: " + std::to_string(p1.tension()), xPosLeftCol, 90, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("BURST: " + std::to_string(p1.burstMeter()), xPosLeftCol, 110, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT STATE: 0z" + ToHexString(static_cast<int>(p1.actionState())), xPosLeftCol, 130, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("GRD STATE: 0z" + ToHexString(static_cast<int>(p1.guardState())), xPosLeftCol, 150, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ATK STATE: 0z" + ToHexString(static_cast<int>(p1.attackState())), xPosLeftCol, 170, zPos, 0xFF, 1.0f);

        auto p1Input = data.GetPlayerInputStructArr()[0].InputRaw;
        fn.RenderCockpitFontText("INPUT: 0z" + ToHexString(static_cast<int>(p1Input)), xPosLeftCol, 190, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("CAM POS: (" + std::to_string(cam.position().x) + "," + std::to_string(cam.position().y) + ")", xPosLeftCol, 220, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("CAM ZOOM: " + std::to_string(cam.zoom()), xPosLeftCol, 240, zPos, 0xFF, 1.0f);

        auto featureFlags = data.GetGameModeFeatureFlags();
        fn.RenderCockpitFontText("MODE FLAGS: 0z" + ToHexString(static_cast<int>(featureFlags)), xPosLeftCol, 260, zPos, 0xFF, 1.0f);
        auto devicePointer = data.GetD3D9Device();
        fn.RenderCockpitFontText("D3D DEVICE: 0z" + ToHexString(reinterpret_cast<int>(devicePointer)), xPosLeftCol, 280, zPos, 0xFF, 1.0f);

        auto playerInputArr = data.GetPlayerInputStructArr();
        
        fn.RenderCockpitFontText("INPUT RAW1: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputRaw)), xPosRightCol, 100, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("INPUT RELEASE: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputRelease)), xPosRightCol, 120, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("INPUT PRESS2: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputPress)), xPosRightCol, 140, zPos, 0xFF, 1.0f);

        fn.RenderMenuText("Menu Text test", xPosRightCol, 200, zPos, 1.0f, nullptr, 0, true, 0xFFFFFF);
        fn.RenderMenuText("Colorful text", xPosRightCol, 220, zPos, 1.0f, nullptr, 0, false, 0xFF8080);
        fn.RenderMenuTextCenterAligned("Centered\nText Test!", xPosRightCol + 60, 240, zPos, 1.0f, 0xFFFFFF, true);

        fn.DrawQuad(610, 10, 630, 30, 1, 0x80FF0000);

        static ggxxacpr::ColorVertex testVerts[4] = {
            {{60.0f, 10.0f, 4.0f}, 0x8FFF0000},
            {{50.0f, 10.0f, 4.0f}, 0x8F00FF00},
            {{60.0f, 20.0f, 4.0f}, 0x8F0000FF},
            {{50.0f, 20.0f, 4.0f}, 0x8FFF8080},
        };
        fn.DrawTriStrip(testVerts, 4);
}
void displayApiTest2(BaseMod::Api& api) {
        constexpr int xPosLeftCol = 10;
        constexpr int xPosRightCol = 340;
        auto& fn = api.NativeFunctions;
        auto& data = api.GameData;

        int32_t err = 0;
        ggxxacpr::Player p1 (data.GetVarAddress<GGXXACPR_Entity>(BaseMod::GameVariable::PLAYER_1, &err));
        if (err) logErr("p1 get", err);
        // auto p1 = data.GetPlayer(0);
        
        ggxxacpr::Player p2 (data.GetVarAddress<GGXXACPR_Entity>(BaseMod::GameVariable::PLAYER_2, &err));
        if (err) logErr("p2 get", err);
        // auto p2 = data.GetPlayer(1);
        
        ggxxacpr::Camera cam(data.GetVarAddress<GGXXACPR_Camera>(BaseMod::GameVariable::CAMERA, &err));
        if (err) logErr("camera get", err);
        // auto cam = data.GetCamera();
        float zPos = static_cast<float>(textZPos);


        fn.RenderCockpitFontText("ACT ID: " + std::to_string(p1.actId()), xPosLeftCol, 10, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT TIMER: " + std::to_string(p1.actTimer()), xPosLeftCol, 30, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("POS: (" + std::to_string(p1.position().x) + "," + std::to_string(p1.position().y) + ")", 10, 50, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("ACT ID: " + std::to_string(p2.actId()), xPosRightCol, 10, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT TIMER: " + std::to_string(p2.actTimer()), xPosRightCol, 30, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("POS: (" + std::to_string(p2.position().x) + "," + std::to_string(p1.position().y) + ")", xPosRightCol, 50, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("HEALTH: " + std::to_string(p1.health()), xPosLeftCol, 70, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("TENSION: " + std::to_string(p1.tension()), xPosLeftCol, 90, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("BURST: " + std::to_string(p1.burstMeter()), xPosLeftCol, 110, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ACT STATE: 0z" + ToHexString(static_cast<int>(p1.actionState())), xPosLeftCol, 130, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("GRD STATE: 0z" + ToHexString(static_cast<int>(p1.guardState())), xPosLeftCol, 150, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("ATK STATE: 0z" + ToHexString(static_cast<int>(p1.attackState())), xPosLeftCol, 170, zPos, 0xFF, 1.0f);

        GGXXACPR_PlayerInput* playerInputArr = data.GetVarAddress<GGXXACPR_PlayerInput>(BaseMod::GameVariable::PLAYER_1_INPUT, &err);
        uint32_t p1Input;
        if (err || !playerInputArr) {
            logErr("p1 input get", err);
        } else {
            p1Input = playerInputArr->InputRaw;
        }
        //auto p1Input = data.GetPlayerInputStructArr()[0].InputRaw;

        fn.RenderCockpitFontText("INPUT: 0z" + ToHexString(static_cast<int>(p1Input)), xPosLeftCol, 190, zPos, 0xFF, 1.0f);

        fn.RenderCockpitFontText("CAM POS: (" + std::to_string(cam.position().x) + "," + std::to_string(cam.position().y) + ")", xPosLeftCol, 220, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("CAM ZOOM: " + std::to_string(cam.zoom()), xPosLeftCol, 240, zPos, 0xFF, 1.0f);

        uint32_t featureFlags;
        uint32_t* addr32 = data.GetVarAddress<uint32_t>(BaseMod::GameVariable::GAME_MODE_FEATURE_FLAGS, &err);
        if (err || !addr32) {
            logErr("get game mode feature flags", err);
        } else {
            featureFlags = *addr32;
        }
        //auto featureFlags = data.GetGameModeFeatureFlags();
        fn.RenderCockpitFontText("MODE FLAGS: 0z" + ToHexString(static_cast<int>(featureFlags)), xPosLeftCol, 260, zPos, 0xFF, 1.0f);
        void* devicePointer = data.GetVarAddress(BaseMod::GameVariable::D3D9_DEVICE, &err);
        if (err) logErr("get D3D device pointer ", err);
        //auto devicePointer = data.GetD3D9Device();
        fn.RenderCockpitFontText("D3D DEVICE: 0z" + ToHexString(reinterpret_cast<int>(devicePointer)), xPosLeftCol, 280, zPos, 0xFF, 1.0f);
        
        fn.RenderCockpitFontText("INPUT RAW1: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputRaw)), xPosRightCol, 100, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("INPUT RELEASE: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputRelease)), xPosRightCol, 120, zPos, 0xFF, 1.0f);
        fn.RenderCockpitFontText("INPUT PRESS2: 0z" + ToHexString(static_cast<int>(playerInputArr[0].InputPress)), xPosRightCol, 140, zPos, 0xFF, 1.0f);
        
        uint16_t bgState;
        uint16_t* addr16 = data.GetVarAddress<uint16_t>(BaseMod::GameVariable::BACKGROUND_STATE, &err);
        if (err || !addr16) {
            logErr("get bg state", err);
        } else {
            bgState = *addr16;
        }

        fn.RenderCockpitFontText("BG STATE: 0z" + ToHexString(static_cast<int>(bgState)), xPosRightCol, 160, zPos, 0xFF, 1.0f);

        fn.RenderMenuText("Menu Text test", xPosRightCol, 200, zPos, 1.0f, nullptr, 0, true, 0xFFFFFF);
        fn.RenderMenuText("Colorful text", xPosRightCol, 220, zPos, 1.0f, nullptr, 0, false, 0xFF8080);
        fn.RenderMenuTextCenterAligned("Centered\nText Test!", xPosRightCol + 60, 240, zPos, 1.0f, 0xFFFFFF, true);

        fn.DrawQuad(610, 10, 630, 30, 1, 0x80FF0000);

        static ggxxacpr::ColorVertex testVerts[4] = {
            {{60.0f, 10.0f, 4.0f}, 0x8FFF0000},
            {{50.0f, 10.0f, 4.0f}, 0x8F00FF00},
            {{60.0f, 20.0f, 4.0f}, 0x8F0000FF},
            {{50.0f, 20.0f, 4.0f}, 0x8FFF8080},
        };
        fn.DrawTriStrip(testVerts, 4);
}
void displayKeyboardInputTest(BaseMod::Api& api) {
    constexpr int x = 200;
    constexpr int y = 225;
    constexpr int xStep = 12;
    constexpr int yStep = 16;

    using Key = ggxxacpr::KeyboardInput;
    struct KeyInfo {
        Key id;
        char label;
    };
    static KeyInfo labels[] {
        {Key::Q, 'Q'},
        {Key::W, 'W'},
        {Key::E, 'E'},
        {Key::R, 'R'},
        {Key::T, 'T'},
        {Key::Y, 'Y'},
        {Key::U, 'U'},
        {Key::I, 'I'},
        {Key::O, 'O'},
        {Key::P, 'P'},

        {Key::A, 'A'},
        {Key::S, 'S'},
        {Key::D, 'D'},
        {Key::F, 'F'},
        {Key::G, 'G'},
        {Key::H, 'H'},
        {Key::J, 'J'},
        {Key::K, 'K'},
        {Key::L, 'L'},
        {Key::SEMI_COLON, ':'},

        {Key::Z, 'Z'},
        {Key::X, 'X'},
        {Key::C, 'C'},
        {Key::V, 'V'},
        {Key::B, 'B'},
        {Key::N, 'N'},
        {Key::M, 'M'},
        {Key::COMMA, ','},
        {Key::PERIOD, '.'},
        {Key::FORWARD_SLASH, '/'},
    };

    for (int i = 0; i < sizeof(labels) / sizeof(KeyInfo); i++) {
        api.NativeFunctions.RenderCockpitFontText(
            &labels[i].label,
            x + (i % 10) * xStep,
            y + (i / 10) * yStep,
            static_cast<float>(textZPos),
            api.GameData.GetKeyboardInput(labels[i].id) ? 0xFF : 0x60,
            1.0f
        );
    }
}

void __stdcall testAfterGameUpdateHook(
    TestContext* uCtx,
    const BaseMod_HookContext* hCtx,
    const BaseMod_GameUpdateInfo* info
) {
    static bool tested = false;

    BaseMod::Api& api = *uCtx->bmApi;

    if (!tested) {
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "AfterGameUpdate hook called");
        tested = true;
    }

    if (api.GameData.IsInGame() &&
        api.GameData.GetJobMode() == ggxxacpr::JobMode::BATTLE
    ) {
        switch(displayData) {
            case 1:
                displayApiTest1(api);
                break;
            case 2:
                displayApiTest2(api);
                break;
            case 3:
                displayKeyboardInputTest(api);
                break;
        }
    }
    if (enableZTest) {
        api.NativeFunctions.DrawQuad(
            0, 0, 640, 480,
            zTestPos,
            enableZTest == 1 ? 0xFF000000 : 0xFF800080
        );
    }
}
void __stdcall testEndSceneGraphicsHook(
    TestContext* uCtx,
    const BaseMod_HookContext* hCtx,
    const BaseMod_DrawInfo* info
) {
    static bool tested = false;
    IDirect3DDevice9* device = reinterpret_cast<IDirect3DDevice9*>(uCtx->bmApi->GameData.GetD3D9Device());
    HRESULT result;

    if (!tested) {
        D3DDEVICE_CREATION_PARAMETERS createParams;
        device->GetCreationParameters(&createParams);
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "EndScene graphics hook called");
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "device pointer from baseMod: 0x" + ToHexString(reinterpret_cast<int>(device)));
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "Device behavior flags: 0x" + ToHexString(createParams.BehaviorFlags));

        IDirect3DDevice9* device = reinterpret_cast<IDirect3DDevice9*>(uCtx->bmApi->GameData.GetD3D9Device());
        tested = true;
    }
    if (displayData == 1 || displayData == 2) {
        static D3DRECT clearRect = D3DRECT { 20, 20, 40, 40 };
        result = device->Clear(1, &clearRect, D3DCLEAR_TARGET, 0xFFFF00FF, 0.0f, 0);
        if (result != D3D_OK) std::cout << "IDirect3DDevice9::Clear failed in End Scene hook" << std::endl;
    }
}
void __stdcall testPresentGraphicsHook(
    TestContext* uCtx,
    const BaseMod_HookContext* hCtx,
    const BaseMod_DrawInfo* info
) {
    static bool tested = false;

    if (!tested) {
        uCtx->glApi->Log(GearLoader::LogLevel::DEBUG, "Present graphics hook called");
        tested = true;
    }

    // Frame time
    static int count = 0;
    static std::chrono::duration<double, std::milli> frameTime;
    static std::chrono::system_clock::time_point lastTimeStamp;
    static double rollingSum = 0.0f;
    
    auto now = std::chrono::high_resolution_clock::now();
    frameTime = now - lastTimeStamp;
    rollingSum += frameTime.count();
    
    if ((count % 60) == 59 ) {
        if (logFrameTime != 0) std::cout << "Avg frame time: " << (rollingSum / 60.0f) << "ms" << std::endl;
        rollingSum = 0.0f;
    }
    
    lastTimeStamp = std::chrono::high_resolution_clock::now();
    count++;
    
    if (displayData == 1 || displayData == 2) {
        IDirect3DDevice9* device = reinterpret_cast<IDirect3DDevice9*>(uCtx->bmApi->GameData.GetD3D9Device());
        D3DVIEWPORT9 viewport { };
        HRESULT result = device->GetViewport(&viewport);
        if (result != D3D_OK) std::cout << "IDirect3DDevice9::GetViewport failed in Present hook" << std::endl;
        static D3DRECT clearRect = D3DRECT {
            static_cast<LONG>(viewport.Width - 40),
            static_cast<LONG>(viewport.Height - 40),
            static_cast<LONG>(viewport.Width - 20),
            static_cast<LONG>(viewport.Height - 20)
        };
        result = device->Clear(1, &clearRect, D3DCLEAR_TARGET, 0xFF00FFFF, 0.0f, 0);
        if (result != D3D_OK) std::cout << "IDirect3DDevice9::Clear failed in Present hook" << std::endl;
    }
}

void BASEMOD_CALL testSaveGameHook(
    TestContext* uCtx,
    const BaseMod::HookContext* hCtx,
    const BaseMod::SaveGameInfo* info
) {
    std::cout << "[BaseMod Tester] Game Saved!" << std::endl;
}

void BASEMOD_CALL TestCommand() {
    std::cout << "TEST COMMAND" << std::endl;
}
void BASEMOD_CALL TriggerPopUp() {
    bmApi->NativeFunctions.RenderPopUpText(0, "POPUP TEST 1");
    bmApi->NativeFunctions.RenderPopUpText(1, "POPUP TEST 2");
}
static int sfxIndex = 0;
void BASEMOD_CALL PlaySFX() {
    bmApi->NativeFunctions.PlayCommonSoundEffect(sfxIndex);
}

void RegisterModMenu(BaseMod::ModMenuApi& api) {
    static int testEnumVal = 0;
    static const char*testEnumValLabels[3] = {
        "VALUE 1", "VALUE 2", "VALUE 3"
    };
    static int testNum = 0;
    static BaseMod::ModMenuEntry entries[13] {
        {"Command Test", nullptr, 0, 0, nullptr, TestCommand},
        {"Enum Test", &testEnumVal, 0, 2, testEnumValLabels, nullptr},
        {"Number Test", &testNum, 0, 100, nullptr, nullptr},
        {"Header", nullptr, 0, 0, nullptr, nullptr},
        {"  Indent 1", nullptr, 0, 0, nullptr, TestCommand},
        {"  Indent 2", nullptr, 0, 0, nullptr, TestCommand},
        {"  Indent 3", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 1", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 2", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 3", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 4", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 5", nullptr, 0, 0, nullptr, TestCommand},
        {"Scroll Test 6", nullptr, 0, 0, nullptr, TestCommand},
    };

    static const char* boolLabels[] = {"OFF", "ON"};
    static const char* dataLabels[] = {"NONE", "GAME DATA", "GAME DATA 2", "KEYBOARD TEST"};
    static const char* zTestLabels[] = {"OFF", "BLACK", "PURPLE"};
    static BaseMod::ModMenuEntry testerEntries[] {
        {"Print frame times", &logFrameTime, 0, 1, boolLabels, nullptr},
        {"Display Data / Tests", &displayData, 0, 3, dataLabels, nullptr},
        {"Data Text Z Pos", &textZPos, 0, 255, nullptr, nullptr},
        {"Trigger Pop-up", nullptr, 0,0, nullptr, TriggerPopUp},
        {"SFX Test", &sfxIndex, 0, 104, nullptr, PlaySFX},
        {"ZPos Test", &enableZTest, 0, 2, zTestLabels},
        {"ZPos Test Level", &zTestPos, 0, 1000},
    };

    api.RegisterMenuTab("TESTER", testerEntries, 7);
    api.RegisterMenuTab("TEST TAB 1", entries, 7);
    api.RegisterMenuTab("TEST TAB 2", entries, 13);
}

GEARLOADER_EXPORT void GEARLOADER_CALL Init(GearLoaderContext* ctx, GearLoaderApi* c_api) {
    GearLoader::Api* glApi = new GearLoader::Api(c_api, ctx);

    const BaseMod_Api* bmApi_c;
    SemanticVersion retVer;
    int result = glApi->RetrieveModApi<BaseMod_Api>(
        BASEMOD_NAME,
        BASEMOD_API_VERSION,
        &bmApi_c,
        &retVer
    );

    if (result > 0) {
        glApi->Log(GearLoader::LogLevel::ERR, "[testMod] failed to retrieve baseMod api");
        return;
    }

    bmApi = new BaseMod::Api(bmApi_c);
    _testCtx = { glApi, bmApi };

    std::stringstream ss1;
    ss1 << "d3d9 device ptr: 0x" << std::hex << bmApi->GameData.GetD3D9Device();
    glApi->Log(GearLoader::LogLevel::DEBUG, ss1.str());

    // TODO: test race condition by waiting
    using namespace std::chrono_literals;
    const auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(1000ms);

    // Test Mod Menu stuff
    RegisterModMenu(bmApi->ModMenu);

    // Test hooks
    bmApi->Hooks.AfterPeekMessage<TestContext>(testPeekMessageHook, &_testCtx);
    bmApi->Hooks.BeforeGameUpdate<TestContext>(testBeforeGameUpdateHook, &_testCtx);
    bmApi->Hooks.AfterGameUpdate<TestContext>(testAfterGameUpdateHook, &_testCtx);
    bmApi->Hooks.BeforeEndScene<TestContext>(testEndSceneGraphicsHook, &_testCtx);
    bmApi->Hooks.BeforePresent<TestContext>(testPresentGraphicsHook, &_testCtx);
    bmApi->Hooks.AfterSaveGame<TestContext>(testSaveGameHook, &_testCtx);

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    std::stringstream ss2;
    ss2 << "Time: " << elapsed.count() << "ms";
    glApi->Log(GearLoader::LogLevel::DEBUG, ss2.str());

    std::cout << "[baseMod Tester] Initialized" << std::endl;
}
