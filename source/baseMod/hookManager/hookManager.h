#pragma once

#include <windows.h>
#include "baseMod_p.h"
#include "gearLoader/gearLoader_c.h"
#include "gearLoaderLogger.h"

const BaseMod_HookApi* GetHookApi();
void InstallHooks(GLLogger* log);
