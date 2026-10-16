#include "gameData.h"
#include "baseMod_p.h"
#include "offsets.h"
#include "charData/charData.h"


// Info about a game variable
struct AddressInfo {
    intptr_t offset;        // from base address
    unsigned short size;    // In bytes
    unsigned short deref;   // How many times should the address be dereferenced
};
static AddressInfo gameVarInfo[] = {
    [BM_VAR_PLAYER_1]                   = {offsets::PLAYER_1_PTR, sizeof(GGXXACPR_Entity), 1},
    [BM_VAR_PLAYER_2]                   = {offsets::PLAYER_2_PTR, sizeof(GGXXACPR_Entity), 1},
    [BM_VAR_PLAYER_1_INPUT]             = {offsets::PLAYER_INPUT_ARR, sizeof(GGXXACPR_PlayerInput), 0},
    [BM_VAR_PLAYER_2_INPUT]             = {offsets::PLAYER_INPUT_ARR_P2, sizeof(GGXXACPR_PlayerInput), 0},
    [BM_VAR_ROOT_ENTITY]                = {offsets::ROOT_ENTITY, sizeof(GGXXACPR_Entity), 0},
    [BM_VAR_CAMERA]                     = {offsets::CAMERA, sizeof(GGXXACPR_Camera), 0},
    [BM_VAR_GAME_VER]                   = {offsets::GAME_VER_FLAG, 4, 0},
    [BM_VAR_JOB_MODE]                   = {offsets::JOB_MODE, 4, 0},
    [BM_VAR_BATTLE_FLAG]                = {offsets::IN_GAME_FLAG, 1, 0},
    [BM_VAR_GAME_MODE_FEATURE_FLAGS]    = {offsets::GAME_MODE_FEATURE_FLAGS, 4, 0},
    [BM_VAR_MAIN_MENU_SELECTION]        = {offsets::MAIN_MENU_SELECTION, 2, 0},
    [BM_VAR_D3D9_DEVICE]                = {offsets::DIRECT3D9_DEVICE, 4, 0},
    [BM_VAR_VIEW_WIDTH]                 = {offsets::VIEW_WIDTH, 4, 0},
    [BM_VAR_VIEW_HEIGHT]                = {offsets::VIEW_HEIGHT, 4, 0},
    [BM_VAR_GLOBAL_THROW_FLAGS]         = {offsets::GLOBAL_THROW_FLAGS, 4, 0},
    [BM_VAR_PAUSE_STATE]                = {offsets::PAUSE_STATE, 4, 0},
    [BM_VAR_PAUSE_DISPLAY_STATE]        = {offsets::PAUSE_DISPLAY_STATE, 4, 0},
    [BM_VAR_LOCALE_STATE]               = {offsets::CURRENT_LOCALE_MODE, sizeof(GGXXACPR_LocaleState), 0},
    [BM_VAR_SURVIVAL_INFO]              = {offsets::SURVIVAL_INFO, sizeof(GGXXACPR_SurvivalInfo), 0},
    [BM_VAR_SHADOW_BOSS_CHECK]          = {offsets::SURVIVAL_SHADOW_BOSS_CHECK, sizeof(GGXXACPR_SurvivalBossEntry), 0},
    [BM_VAR_GOLDEN_BOSS_CHECK]          = {offsets::SURVIVAL_GOLDEN_BOSS_CHECK, sizeof(GGXXACPR_SurvivalBossEntry), 0},
    [BM_VAR_BACKGROUND_STATE]           = {offsets::BACKGROUND_STATE, 2, 0},
    [BM_VAR_KEYBOARD_INPUT]             = {offsets::KEYBOARD_INPUT, 256, 0},
};
constexpr int NUM_VARS = sizeof(gameVarInfo) / sizeof(AddressInfo);

void* BASEMOD_CALL GetVarAddress(int32_t varId, int32_t* errOut) {
    if (varId < 0 || NUM_VARS <= varId) {
        if (errOut) *errOut = 1;
        return 0;
    }

    AddressInfo& varInfo = gameVarInfo[varId];
    std::byte* addr = getBaseAddress() + varInfo.offset;

    for (int i = 0; i < varInfo.deref; i++) {
        if (!addr) {    // Basic null pointer check
            if (errOut) *errOut = 2;
            return 0;
        }
        // Dangerous pointer deref, varInfo better be correct!
        addr = *reinterpret_cast<std::byte**>(addr);
    }

    return addr;
}


GGXXACPR_Entity* BASEMOD_CALL GetPlayer(int playerIndex) {
    std::byte *address = getBaseAddress() + (playerIndex == 0 ?
        offsets::PLAYER_1_PTR :
        offsets::PLAYER_2_PTR);
    return *reinterpret_cast<GGXXACPR_Entity**>(address);
}
GGXXACPR_RawControllerInput BASEMOD_CALL GetPlayerInput(int playerIndex) {
    std::byte *address = getBaseAddress() + offsets::PLAYER_INPUT + (playerIndex * 0xA8);
    return *reinterpret_cast<GGXXACPR_RawControllerInput*>(address);
}
GGXXACPR_Camera* BASEMOD_CALL GetCamera() {
    std::byte *address = getBaseAddress() + offsets::CAMERA;
    return reinterpret_cast<GGXXACPR_Camera*>(address);
}
int32_t BASEMOD_CALL IsInGame() {
    std::byte *address = getBaseAddress() + offsets::IN_GAME_FLAG;
    uint8_t* flagPtr = reinterpret_cast<uint8_t*>(address);
    return (*flagPtr) != 0;
}
int32_t BASEMOD_CALL GetJobMode() {
    std::byte *address = getBaseAddress() + offsets::JOB_MODE;
    return *reinterpret_cast<int32_t*>(address);
}
uint32_t BASEMOD_CALL GetGameModeFeatureFlags() {
    std::byte *address = getBaseAddress() + offsets::GAME_MODE_FEATURE_FLAGS;
    return *reinterpret_cast<uint32_t*>(address);
}
uint32_t BASEMOD_CALL GetMainMenuSelection() {
    std::byte *address = getBaseAddress() + offsets::MAIN_MENU_SELECTION;
    return *reinterpret_cast<uint32_t*>(address);
}
void* BASEMOD_CALL GetD3D9Device() {
    return *reinterpret_cast<void**>(getBaseAddress() + offsets::DIRECT3D9_DEVICE);
}
uint32_t BASEMOD_CALL GetGameVersion() {
    return *reinterpret_cast<uint32_t*>(getBaseAddress() + offsets::GAME_VER_FLAG);
}
uint32_t BASEMOD_CALL GetViewWidth() {
    return *reinterpret_cast<uint32_t*>(getBaseAddress() + offsets::VIEW_WIDTH);
}
uint32_t BASEMOD_CALL GetViewHeight() {
    return *reinterpret_cast<uint32_t*>(getBaseAddress() + offsets::VIEW_HEIGHT);
}
GGXXACPR_Entity* BASEMOD_CALL GetRootEntity() {
    return reinterpret_cast<GGXXACPR_Entity*>(getBaseAddress() + offsets::ROOT_ENTITY);
}
uint32_t BASEMOD_CALL GetGlobalThrowFlags() {
    return *reinterpret_cast<uint32_t*>(getBaseAddress() + offsets::GLOBAL_THROW_FLAGS);
}
int32_t* BASEMOD_CALL GetPauseState() {
    return reinterpret_cast<int32_t*>(getBaseAddress() + offsets::PAUSE_STATE);
}
int32_t* BASEMOD_CALL GetPauseDisplayState() {
    return reinterpret_cast<int32_t*>(getBaseAddress() + offsets::PAUSE_DISPLAY_STATE);
}
GGXXACPR_PlayerInput* BASEMOD_CALL GetPlayerInputStructArr() {
    return reinterpret_cast<GGXXACPR_PlayerInput*>(getBaseAddress() + offsets::PLAYER_INPUT_ARR);
}
GGXXACPR_LocaleState* BASEMOD_CALL GetLocaleState() {
    return reinterpret_cast<GGXXACPR_LocaleState*>(getBaseAddress() + offsets::CURRENT_LOCALE_MODE);
}
GGXXACPR_SurvivalInfo* BASEMOD_CALL GetSurvivalInfo() {
    return reinterpret_cast<GGXXACPR_SurvivalInfo*>(getBaseAddress() + offsets::SURVIVAL_INFO);
}
GGXXACPR_SurvivalBossEntry* BASEMOD_CALL GetShadowBossCheck() {
    return reinterpret_cast<GGXXACPR_SurvivalBossEntry*>(getBaseAddress() + offsets::SURVIVAL_SHADOW_BOSS_CHECK);
}
GGXXACPR_SurvivalBossEntry* BASEMOD_CALL GetGoldenBossCheck() {
    return reinterpret_cast<GGXXACPR_SurvivalBossEntry*>(getBaseAddress() + offsets::SURVIVAL_GOLDEN_BOSS_CHECK);
}
uint8_t BASEMOD_CALL GetKeyboardInput(int32_t keyId) {
    if (keyId < 0 || 256 <= keyId) return 0;
    return reinterpret_cast<uint8_t*>(getBaseAddress() + offsets::KEYBOARD_INPUT)[keyId];
}

const BaseMod_GameDataApi* GetGameDataApi() {
    static const BaseMod_GameDataApi _gameDataApi = {
        size: sizeof(BaseMod_GameDataApi),
        version: BASEMOD_API_VERSION_NUM,
        
        CharacterData: GetCharDataApi(),

        GetPlayer: GetPlayer,
        GetPlayerInputStructArr: GetPlayerInputStructArr,
        GetCamera: GetCamera,
        IsInGame: IsInGame,
        GetJobMode: GetJobMode,
        GetGameModeFeatureFlags: GetGameModeFeatureFlags,
        GetMainMenuSelection: GetMainMenuSelection,
        GetD3D9Device: GetD3D9Device,
        GetGameVersion: GetGameVersion,
        GetViewWidth: GetViewWidth,
        GetViewHeight: GetViewHeight,
        GetRootEntity: GetRootEntity,
        GetGlobalThrowFlags: GetGlobalThrowFlags,
        GetPauseState: GetPauseState,
        GetPauseDisplayState: GetPauseDisplayState,
        GetLocaleState: GetLocaleState,
        GetSurvivalInfo: GetSurvivalInfo,
        GetShadowBossCheck: GetShadowBossCheck,
        GetGoldenBossCheck: GetGoldenBossCheck,
        GetKeyboardInput: GetKeyboardInput,
        GetVarAddress: GetVarAddress,
    };

    return &_gameDataApi;
}
