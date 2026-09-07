#include "baseMod_p.h"
#include <iostream>
#include <windows.h>
#include <cstddef>
#include "gearLoader/gearLoader_c.h"
#include "gearLoader/ggxxacpr_c.h"
#include "nativeFunctions/nativeFunctions.h"
#include "gameData/gameData.h"
#include "hookManager/hookManager.h"
#include "gearLoaderLogger.h"
#include "modMenu/modMenu.h"


SemanticVersion getSemVer() {
    unsigned int verNum = BASEMOD_API_VERSION_NUM;
    return {
        (verNum & 0xFF0000) >> 16,
        (verNum & 0x00FF00) >> 8,
        (verNum & 0x0000FF),
    };
}

std::byte* getBaseAddress() {
    static std::byte* _baseAddress = reinterpret_cast<std::byte*>(GetModuleHandle(nullptr));
    return _baseAddress;
}

static const BaseMod_Api _api = {
    sizeof(BaseMod_Api),
    BASEMOD_API_VERSION_NUM,

    GetNativeFunctionsApi(),
    GetGameDataApi(),
    GetHookApi(),
    GetModMenuApi()
};

GEARLOADER_EXPORT void GEARLOADER_CALL Init(GearLoaderContext* ctx, GearLoaderApi* modLoaderApi) {
    GLLogger* logger = new GLLogger(ctx, modLoaderApi);
    InstallHooks(logger);
    InstallModMenu();
    modLoaderApi->RegisterApi(ctx, &_api, BASEMOD_NAME, getSemVer());
    std::cout << "[baseMod] Initialized" << std::endl;
}
