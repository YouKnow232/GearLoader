#include "gameData.h"
#include "baseMod_p.h"
#include "offsets.h"
#include "charData/charData.h"


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
GGXXACPR_SurvivalInfo BASEMOD_CALL GetSurvivalInfo() {
    return reinterpret_cast<GGXXACPR_SurvivalInfo>(getBaseAddress() + offsets::SURVIVAL_INFO);
}
GGXXACPR_SurvivalBossEntry* BASEMOD_CALL GetShadowBossCheck() {
    return reinterpret_cast<GGXXACPR_SurvivalBossEntry*>(getBaseAddress() + offsets::SURVIVAL_SHADOW_BOSS_CHECK);
}
GGXXACPR_SurvivalBossEntry* BASEMOD_CALL GetGoldenBossCheck() {
    return reinterpret_cast<GGXXACPR_SurvivalBossEntry*>(getBaseAddress() + offsets::SURVIVAL_GOLDEN_BOSS_CHECK);
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
    };

    return &_gameDataApi;
}
