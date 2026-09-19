#include "modMenu.h"
#include <cstdint>
#include <windows.h>
#include <iostream>
#include <vector>
#include "offsets.h"
#include "gearLoader/ggxxacpr.hpp"
#include "nativeFunctions/nativeFunctions.h"
#include "gameData/gameData.h"
#include "patch.h"

using namespace ggxxacpr;
using Input = RawControllerInput;
using ModeFlag = GameModeFeatureFlags;

struct ButtonMapping {
    int32_t PunchKey;
    int32_t KickKey;
    int32_t SlashKey;
    int32_t HeavySlashKey;
    int32_t DustKey;
    int32_t FuckinKey;
    int32_t PosResetKey;
    int32_t PauseKey;
    int32_t RecPlayerKey;
    int32_t RecEnemyKey;
    int32_t PlayMemoryKey;
    int32_t SwitchKey;
    int32_t EnemyWalkKey;
    int32_t EnemyJumpKey;
    int32_t PunchAndKickKey;
    int32_t PunchAndDustKey;
    int32_t PKSKey;
    int32_t PKSHSKey;
    int32_t _padding1;
    int32_t _padding2;
};


inline intptr_t base() {
    static intptr_t _base = reinterpret_cast<intptr_t>(GetModuleHandle(NULL));
    return _base;
}
inline auto Native() {
    static auto api = GetNativeFunctionsApi();
    return api;
}
inline auto GameData() {
    static auto api = GetGameDataApi();
    return api;
}
inline auto PlayerInputArr() {
    static auto pInput = GameData()->GetPlayerInputStructArr();
    return pInput;
}
inline auto ModeFlags() {
    return static_cast<GameModeFeatureFlags>(GameData()->GetGameModeFeatureFlags());
}
inline auto PauseState() {
    static auto pauseState = GameData()->GetPauseState();
    return pauseState;
}
inline auto Locale() {
    static auto locale = GameData()->GetLocaleState();
    return locale;
}

// Forward declaration for ModMenu
const BaseMod_ModMenu_HelperFunctionsApi* GetHelperFunctionApi();

//////////////////////////
// PLACE HOLDER GLOBALS //
//////////////////////////

// globals:
uint32_t* pause_menu_selection = reinterpret_cast<uint32_t*>(base() + offsets::PAUSE_MENU_SELECTION);
uint8_t* sub_menu_is_open = reinterpret_cast<uint8_t*>(base() + offsets::SUB_MENU_IS_OPEN);
PlayerInput* input_struct_2 = reinterpret_cast<PlayerInput*>(base() + offsets::INPUT_STRUCT_2);
int32_t* menu_input_hold_timer = reinterpret_cast<int32_t*>(base() + offsets::MENU_INPUT_HOLD_TIMER);
ButtonMapping* button_mappings_1 = reinterpret_cast<ButtonMapping*>(base() + offsets::BUTTON_MAPPINGS_1);
ButtonMapping* button_mappings_2 = reinterpret_cast<ButtonMapping*>(base() + offsets::BUTTON_MAPPINGS_2);
int32_t* is_past_menu = reinterpret_cast<int32_t*>(base() + offsets::IS_PAST_MENU);
int32_t* unknown_struct_field = reinterpret_cast<int32_t*>(base() + offsets::ASTRUCT_5_FIELD);
void* KSET_controller_settings_fiber = reinterpret_cast<void*>(base() + offsets::KSET_FIBER_FUNCTION);
void* KBST_controller_settings_fiber = reinterpret_cast<void*>(base() + offsets::KBST_FIBER_FUNCTION);
void* INSD_controller_settings_fiber = reinterpret_cast<void*>(base() + offsets::INSD_FIBER_FUNCTION);
BOOL* disable_primary_menu = reinterpret_cast<BOOL*>(base() + offsets::DISABLE_PRIMARY_MENU);
BOOL* display_confirm_exit_dialog = reinterpret_cast<BOOL*>(base() + offsets::DISPLAY_CONFIRM_EXIT_DIALOG);
DrawSpriteParams* menu_scroll_up_arrow = reinterpret_cast<DrawSpriteParams*>(base() + offsets::MENU_SCROLL_UP_ARROW_SPRITE);
DrawSpriteParams* menu_scroll_down_arrow = reinterpret_cast<DrawSpriteParams*>(base() + offsets::MENU_SCROLL_DOWN_ARROW_SPRITE);
LPCWCH** locale_dictionaries_unicode = reinterpret_cast<LPCWCH**>(base() + offsets::STRING_DICTIONARY_UNICODE);
LPCCH** locale_dictionaries = reinterpret_cast<LPCCH**>(base() + offsets::STRING_DICTIONARY);

// HLOP:
using VoidFn = void(__stdcall*)();
using CheckFn = bool(__stdcall*)();
VoidFn create_CTRLS_fiber = reinterpret_cast<VoidFn>(base() + offsets::CTRLS_FIBER_ENTRY);
VoidFn create_HOWTO_fiber = reinterpret_cast<VoidFn>(base() + offsets::HOWTO_FIBER_ENTRY);
VoidFn create_GLSFR_fiber = reinterpret_cast<VoidFn>(base() + offsets::GLSFR_FIBER_ENTRY);
VoidFn create_GLSFR_ggpo_fiber = reinterpret_cast<VoidFn>(base() + offsets::GLSFR_GGPO_FIBER_ENTRY);
CheckFn check_CTRLS_fiber = reinterpret_cast<CheckFn>(base() + offsets::CTRLS_CHECK_FN);
CheckFn check_HOWTO_fiber = reinterpret_cast<CheckFn>(base() + offsets::HOWTO_CHECK_FN);
CheckFn check_GLSFR_fiber = reinterpret_cast<CheckFn>(base() + offsets::GLSFR_CHECK_FN);
CheckFn check_GLSFR_ggpo_fiber = reinterpret_cast<CheckFn>(base() + offsets::GLSFR_CHECK_FN);


static GGXXACPR_DrawSpriteParams scrollUpArrow = {
    0x100,                  // spriteId
    32.0f, 123.0f, 1.0f,    // x, y, z
    1.0f, 1.0f,             // zoom
    0.0f, 0.0f, 1.0f, 1.0f, // UVs
    0,                      // angle
    1.0f,                   // transl
    1, 0x2A, 0xFFFFFF, 0,   // list/attr/colors
    0, 0,                   // size
    0                       // texture
};
static GGXXACPR_DrawSpriteParams scrollDownArrow = {
    0x100,                  // spriteId
    32.0f, 412.0f, 1.0f,    // x, y, z
    1.0f, 1.0f,             // zoom
    0.0f, 0.0f, 1.0f, 1.0f, // UVs
    0,                      // angle
    1.0f,                   // transl
    1, 0x0A, 0xFFFFFF, 0,   // list/attr/colors
    0, 0,                   // size
    0                       // texture
};


// fiber stuff:
void** main_fiber = reinterpret_cast<void**>(base() + offsets::MAIN_FIBER);
int32_t* fiber_counter = reinterpret_cast<int32_t*>(base() + offsets::FIBER_COUNTER);
int32_t* frame_counter = reinterpret_cast<int32_t*>(base() + offsets::FRAME_COUNTER);

enum CommonSoundEffectIds {
    SE_GAUGE    = 0x06,
    SE_SELECT   = 0x37,
    SE_ACCEPT   = 0x39,
    SE_EXIT     = 0x3B,
};


//////////////////////
// Helper Functions //
//////////////////////

inline bool NeitherInputting(Input input) {
    return !(PlayerInputArr()[0].InputRaw & static_cast<uint32_t>(input)) &&
        !(PlayerInputArr()[1].InputRaw & static_cast<uint32_t>(input));
}
inline bool EitherInputting(Input input) {
    return !NeitherInputting(input);
}
inline bool NeitherPressed(Input input) {
    return !(PlayerInputArr()[0].PrevInputPress & static_cast<uint32_t>(input)) &&
        !(PlayerInputArr()[1].PrevInputPress & static_cast<uint32_t>(input));
}
inline bool EitherPressed(Input input) {
    return !NeitherPressed(input);
}

inline int WhichPlayerIsInputting(Input input) {
    if (PlayerInputArr()[0].InputRaw & static_cast<uint32_t>(input)) {
        return 1;
    }
    if (PlayerInputArr()[1].InputRaw & static_cast<uint32_t>(input)) {
        return 2;
    }
    return 0;
}



int32_t BASEMOD_CALL HandleMenuSelection(int32_t selection, uint32_t totalEntries) {
    constexpr uint32_t up = static_cast<uint32_t>(Input::UP);
    constexpr uint32_t down = static_cast<uint32_t>(Input::DOWN);

    auto p1Input = PlayerInputArr()[0].InputRaw;
    auto p2Input = PlayerInputArr()[1].InputRaw;

    if (NeitherInputting(Input::UP)) {
        if (NeitherInputting(Input::DOWN)) {
            menu_input_hold_timer[1] = 0;
            menu_input_hold_timer[2] = 0;
        } else {
            int holdTimerIndex = 0;
            if (!(p1Input & down)) {
                holdTimerIndex = 1;
            } else if (!(p2Input & down)) {
                holdTimerIndex = 2;
            }
            int holdTimer = menu_input_hold_timer[holdTimerIndex];
            // if input is held for 16 frames or more repeat every third frame
            if (holdTimer == 0 || (holdTimer > 16 && ((holdTimer & 3) == 0))) {
                Native()->PlayCommonSoundEffect(SE_SELECT);
                selection++;
                if (selection > totalEntries - 1) {
                    selection = 0;
                }
            }
            menu_input_hold_timer[holdTimerIndex]++;
        }
    } else {
        int holdTimerIndex = 0;
        if (!(p1Input & up)) {
            holdTimerIndex = 1;
        } else if (!(p2Input & up)) {
            holdTimerIndex = 2;
        }
        int holdTimer = menu_input_hold_timer[holdTimerIndex];
        // if input is held for 16 frames or more repeat every third frame
        if (holdTimer == 0 || (holdTimer > 16 && ((holdTimer & 3) == 0))) {
            Native()->PlayCommonSoundEffect(SE_SELECT);
            selection--;
            if (selection < 0) {
                selection = totalEntries - 1;
            }
        }
        menu_input_hold_timer[holdTimerIndex]++;
    }
    return selection;
}

int32_t BASEMOD_CALL HoldDirectionInputHandler(uint32_t input) {
    constexpr uint32_t directionInputMask = 0xF0;

    uint32_t p1Input = PlayerInputArr()[0].InputRaw;
    uint32_t p2Input = PlayerInputArr()[1].InputRaw;

    if ((p1Input & input) || (p2Input & input)) {
        int pIndex = ((p1Input & input) != 0) ? 0 : 1;
        int holdTimer = 0;
        switch (input) {
            case INPUT_LEFT:
                holdTimer = PlayerInputArr()[pIndex].LeftCharge;
                break;
            case INPUT_UP:
                holdTimer = PlayerInputArr()[pIndex].UpCharge;
                break;
            case INPUT_RIGHT:
                holdTimer = PlayerInputArr()[pIndex].RightCharge;
                break;
            case INPUT_DOWN:
                holdTimer = PlayerInputArr()[pIndex].DownCharge;
                break;
        }
        // if input is held for 16 frames or more repeat every third frame
        return holdTimer == 1 || (holdTimer > 16 && ((holdTimer & 3) == 1));
    }

    return 0;
}
inline bool HoldDirectionInputHandler(Input input) {
    return HoldDirectionInputHandler(static_cast<uint32_t>(input));
}

void BASEMOD_CALL DrawEnumSettingUI(const char* label, int32_t xOffset, int32_t yPos, int32_t isSelected) {
    constexpr int32_t leftArrowX = 332;
    constexpr int32_t rightArrowX = 529;
    constexpr int32_t arrowYOffset = 13;
    constexpr int32_t labelYOffset = 5;
    constexpr int valueX = 430;
    Native()->DrawArrowSprite(BM_DASD_LEFT, leftArrowX + xOffset, yPos + arrowYOffset, 2, isSelected ? 0x01 : 0x50);
    Native()->RenderCockpitFontText(
        label,
        valueX + xOffset - strlen(label) * 6,
        yPos + labelYOffset,
        2.0f,
        isSelected ? 0xFF : 0xA0,
        1.0f
    );
    Native()->DrawArrowSprite(BM_DASD_RIGHT, rightArrowX + xOffset, yPos + arrowYOffset, 2, isSelected ? 0x01 : 0x50);
}
// Invokes the native draw gauge setting function. Notably, the xPos for the graphic is hardcoded.
void BASEMOD_CALL DrawGaugeSettingUI(int32_t currentValue, int32_t yPos, int32_t isSelected, int32_t maxValue) {
    Native_MenuEntry native_entry = {
        nullptr, 0, 0,  // unused
        currentValue,
        0, nullptr      // unused
    };
    draw_gauge_setting_ui(yPos, &native_entry, isSelected ? 0xFF : 0x50, maxValue);
}

void BASEMOD_CALL DrawScrollArrow(int32_t flags) {
    if ((flags & BM_DSAF_UP) != 0)
        Native()->DrawSprite(&scrollUpArrow, 0);
    if ((flags & BM_DSAF_DOWN) != 0)
        Native()->DrawSprite(&scrollDownArrow, 0);
}

void BASEMOD_CALL SwitchToMainFiber() {
    FiberData* fData = reinterpret_cast<FiberData*>(GetFiberData());
    if (fData == nullptr) {
        *fiber_counter = *frame_counter + 1;
    } else {
        fData->Waiting = 1;
        SwitchToFiber(*main_fiber);
    }
}

inline void DrawModMenuHeaderText(const char* text, float x, float y, uint32_t alpha, float maxSize, float maxWidth) {
    constexpr int glyphWidth = 21;
    constexpr int glyphHeight = 17;
    int strLength = strlen(text);
    float scale = std::min(maxWidth / (strLength * glyphWidth), maxSize);
    float adjustedX = x - strLength * glyphWidth * scale / 2.0f;
    float adjustedY = y - glyphHeight * scale / 2.0f;
    draw_menu_header_font(
        text,
        adjustedX, adjustedY, 2.0f,
        alpha, 6, scale
    );
}


//////////////
// Mod Menu //
//////////////

struct MenuTab {
    const char* Title;
    const BaseMod_ModMenuEntry* Entries;
    uint32_t NumEntries;
    BM_CustomMenuHandler customHandler;
};
static std::vector<MenuTab> _modMenuTabs;

// I'll be honest, I don't really know what is going on here
void EnsureScrollArrowSpriteLoaded() {
    static int* loadSign = reinterpret_cast<int*>(base() + offsets::SCROLL_ARROW_LOAD_SYMBOL);
    if (loadSign[6] == -1) {
        // Training Menu init routine
        create_fiber(reinterpret_cast<void*>(base() + offsets::TRAINING_MENU_INIT_FN), 0x2000, 2, 1, "INITM");
    }
}

void __stdcall ModMenu() {
    constexpr uint32_t maxVisibleEntries = 10;
    constexpr uint32_t rightFace = static_cast<uint32_t>(Input::RIGHT_FACE);

    static int selection = 0;
    static int tab = 0;
    static int scrollOffset = 0;

    while (NeitherPressed(Input::RIGHT_FACE)) {
        SwitchToMainFiber();

        // Tab selection
        if (_modMenuTabs.size() > 1) {
            if (EitherPressed(Input::R1)) {
                Native()->PlayCommonSoundEffect(SE_SELECT);
                tab++;
                if (tab > _modMenuTabs.size() - 1) tab = 0;
                selection = 0;
                scrollOffset = 0;
            } else if (EitherPressed(Input::L1)) {
                Native()->PlayCommonSoundEffect(SE_SELECT);
                tab--;
                if (tab < 0) tab = _modMenuTabs.size() - 1;
                selection = 0;
                scrollOffset = 0;
            }
        }

        // Invoke custom handler if applicable
        if (_modMenuTabs[tab].customHandler) {
            _modMenuTabs[tab].customHandler(PlayerInputArr());
        }
        // This block encompasses functionality and drawing code for the menu entries
        if (_modMenuTabs[tab].Entries) {
            // Entry selection
            if (_modMenuTabs[tab].NumEntries > 1) {
                selection = HandleMenuSelection(selection, _modMenuTabs[tab].NumEntries);
                if (selection < scrollOffset) scrollOffset = selection;
                if (selection > scrollOffset + maxVisibleEntries - 1) scrollOffset = selection - maxVisibleEntries + 1;
            }

            BaseMod_ModMenuEntry entry = _modMenuTabs[tab].Entries[selection];
            bool entryIsGauge = entry.ValueLabels == nullptr;

            // Entry functionality
            if (EitherPressed(Input::BOTTOM_FACE) && entry.Command) {
                Native()->PlayCommonSoundEffect(SE_ACCEPT);
                entry.Command();
            }
            if (entry.Value) {
                if (HoldDirectionInputHandler(Input::LEFT)) {
                    int increment = 1;
                    if (entry.ValueLabels) {
                        Native()->PlayCommonSoundEffect(SE_SELECT);
                    } else {
                        Native()->PlayCommonSoundEffect(SE_GAUGE);
                        increment = EitherInputting(Input::BOTTOM_FACE) ? 10 : 1;
                    }
                    int val = (*entry.Value - increment);
                    if (val < entry.MinValue) val = entry.ValueLabels ? entry.MaxValue : entry.MinValue;
                    *entry.Value = val;
                    if (entry.ValueChanged) entry.ValueChanged(val);
                } else if (HoldDirectionInputHandler(Input::RIGHT)) {
                    int increment = 1;
                    if (entry.ValueLabels) {
                        Native()->PlayCommonSoundEffect(SE_SELECT);
                    } else {
                        Native()->PlayCommonSoundEffect(SE_GAUGE);
                        increment = EitherInputting(Input::BOTTOM_FACE) ? 10 : 1;
                    }
                    int val = (*entry.Value + increment);
                    if (val > entry.MaxValue) val = entry.ValueLabels ? entry.MinValue : entry.MaxValue;
                    *entry.Value = val;
                    if (entry.ValueChanged) entry.ValueChanged(val);
                }
            }

            // Entry drawing logic
            if (_modMenuTabs[tab].NumEntries > maxVisibleEntries) {
                if (scrollOffset > 0)
                    Native()->DrawSprite(&scrollUpArrow, 0);
                if (scrollOffset < _modMenuTabs[tab].NumEntries - maxVisibleEntries)
                    Native()->DrawSprite(&scrollDownArrow, 0);
            }
            
            constexpr int labelX = 32;
            constexpr int leftArrowX = 334;
            constexpr int valueX = 430;
            constexpr int rightArrowX = 529;
            constexpr int arrowYOffset = 8;
            constexpr int baseY = 111;
            for (int i = 0; i < std::min(_modMenuTabs[tab].NumEntries, maxVisibleEntries); i++) {
                int iEntry = i + scrollOffset;
                int yPos = baseY + 0x20 * i;
                int32_t alpha = selection == iEntry ? 0xFF : 0x9F;
                entry = _modMenuTabs[tab].Entries[iEntry];

                Native()->RenderMenuText(
                    entry.Label,
                    labelX, yPos, 2.0f,
                    255.0f / alpha,
                    nullptr, 0, 0, 0xFFFFFFFF);

                if (entry.ValueLabels) {
                    DrawEnumSettingUI(
                        entry.ValueLabels[*entry.Value],
                        0,
                        yPos,
                        selection == iEntry
                    );
                } else if (entry.Value) {
                    DrawGaugeSettingUI(*entry.Value, yPos, selection == iEntry, entry.MaxValue);
                }
            }
        } // End menu entry code

        // Header drawing logic
        constexpr float centerHeaderX = 320.0f;
        constexpr float centerHeaderY = 53.0f;
        constexpr float centerHeaderMaxWidth = 600.0f;
        constexpr float sideHeaderXOffset = 160.0f;
        constexpr float sideHeaderYOffset = -10.0f;
        constexpr float sideHeaderMaxWidth = 280.0f;
        constexpr float sideHeaderScale = 0.75f;
        if (tab - 1 >= 0) { // Left header
            DrawModMenuHeaderText(
                _modMenuTabs[tab-1].Title,
                centerHeaderX - sideHeaderXOffset,
                centerHeaderY + sideHeaderYOffset,
                128, sideHeaderScale, sideHeaderMaxWidth
            );
        }
        // Center header
        DrawModMenuHeaderText(
            _modMenuTabs[tab].Title,
            centerHeaderX, centerHeaderY,
            1, 1.0f, centerHeaderMaxWidth
        );
        if (_modMenuTabs.size() > 1) {
            // pips
            constexpr int pipSpacing = 5;
            constexpr int pipY = centerHeaderY + 12;
            int pipBaseX = centerHeaderX - (_modMenuTabs.size() * pipSpacing / 2);
            for (int i = 0; i < _modMenuTabs.size(); i++) {
                Native()->RenderMenuText(
                    ".",
                    pipBaseX + i * pipSpacing,
                    pipY, 2.0f,
                    tab == i ? 1.0f : 0.4f,
                    nullptr, 0,
                    0, 0xFFFFFFFF
                );
            }
        }
        if ((tab + 1) < _modMenuTabs.size()) { // Right header
            DrawModMenuHeaderText(
                _modMenuTabs[tab+1].Title,
                centerHeaderX + sideHeaderXOffset,
                centerHeaderY + sideHeaderYOffset,
                128, sideHeaderScale, sideHeaderMaxWidth
            );
        }
        // Header bg
        Native()->DrawQuad(0, 85, 640,  86, 4, 0xFFCC0000); // red top line
    }

    // Cleanup
    Native()->PlayCommonSoundEffect(SE_EXIT);
    *sub_menu_is_open = 0;
}

bool __stdcall ModMenuFiberExists() {
    return does_fiber_exist("MOD_MENU");
}

void __stdcall create_MOD_MENU_fiber() {
    if (!does_fiber_exist("MOD_MENU")) {
        EnsureScrollArrowSpriteLoaded();
        create_fiber(reinterpret_cast<void*>(ModMenu), 0x2000, 1, 0x12442, "MOD_MENU");
    }
}


///////////////
// Menu Hook //
///////////////

struct HLOPMenuEntry {
    int labelId;
    void (__stdcall*FiberEntryFunc)();
    bool (__stdcall*CheckFunc)();
};
void HLOP_fiber_entry_replacement() {
    constexpr int numEntries = 5;
    static int32_t* jobMode = reinterpret_cast<int32_t*>(base() + offsets::JOB_MODE);
    int selection = 0;
    HLOPMenuEntry entries[numEntries] = {
        {0, create_MOD_MENU_fiber, ModMenuFiberExists},
        {0x32F, create_CTRLS_fiber, check_CTRLS_fiber},
        {0x32E, create_HOWTO_fiber, check_HOWTO_fiber},
        {0x330, create_GLSFR_fiber, check_GLSFR_fiber},
        {0x331, create_GLSFR_ggpo_fiber, check_GLSFR_ggpo_fiber},
    };
    bool subMenuRunning = false;

    do {
        FiberData* fData = reinterpret_cast<FiberData*>(GetFiberData());
        if (fData == nullptr ||
            (*jobMode == JOB_MODE_BATTLE && *PauseState() == 0)
        ) {
            *fiber_counter = *frame_counter + 1;
        } else {
            fData->Waiting = 1;
            SwitchToFiber(*main_fiber);
        }
        subMenuRunning = entries[selection].CheckFunc();
        const char* modSettingsStr = _modMenuTabs.size() == 0 ? "MOD SETTINGS (NONE)" : "MOD SETTINGS";

        // skip drawing if mod menu is open
        if (ModMenuFiberExists()) continue;

        // Draw labels
        int yPos = 166;
        for(int i = 0; i < numEntries; i++) {
            int label = entries[i].labelId;
            Native()->RenderMenuText(
                label == 0 ? modSettingsStr : get_string(Locale(), label),
                0xC0, yPos, 2.0f,
                selection == i ? 1.0f : (255.0f / 160.0f),
                nullptr, 0, 0,
                0xFFFFFFFF);
            yPos += 0x20;
        }

        // Skip selection & functionality
        if (subMenuRunning) continue;

        selection = HandleMenuSelection(selection, numEntries);

        // Functionality
        if (EitherPressed(Input::BOTTOM_FACE)) {
            if (selection == 0 && _modMenuTabs.size() == 0) {
                Native()->PlayCommonSoundEffect(SE_EXIT);
            } else {
                Native()->PlayCommonSoundEffect(SE_ACCEPT);
                entries[selection].FiberEntryFunc();
                subMenuRunning = true;
            }
        }
    } while (NeitherPressed(Input::RIGHT_FACE) || subMenuRunning);

    Native()->PlayCommonSoundEffect(SE_EXIT);
}

int update_generic_pause_menu_substitute() {
    constexpr int itemsLength = 7;
    constexpr int baseY = 0x88;
    static Native_MenuEntry items[itemsLength] = {
        {"RESUME GAME", 0xD0, baseY, 0, 0, nullptr},
        {"CONTROLLER SETTINGS", 0xD0, baseY + 0x20, 0, 0, nullptr},
        {"KEYBOARD SETTINGS", 0xD0, baseY+0x40, 0, 0, nullptr},
        {"COMMAND LIST", 0xD0, baseY+0x60, 0, 0, nullptr},
        {"HELP AND OPTIONS", 0xD0, baseY+0x80, 0, 0, nullptr},
        {"MOD SETTINGS", 0xD0, baseY+0xA0, 0, 0, nullptr},
        {"EXIT GAME", 0xD0, baseY+0xC0, 0, 0, nullptr},
    };
    static int strIds[itemsLength] = {0x332, 0x333, 0x9fb, 0x334, 0x32d, 0, 0x337};

    int selection = *pause_menu_selection;
    auto p1Input = PlayerInputArr()[0].InputRaw;
    auto p2Input = PlayerInputArr()[1].InputRaw;

    if (does_fiber_exist("HLOP") || *sub_menu_is_open) {
        return selection;
    }

    selection = HandleMenuSelection(selection, itemsLength);
    
    ModeFlag freePlayFlag = ModeFlags() & ModeFlag::FREE_PLAY;
    PlayerInput* input = input_struct_2;
    ButtonMapping* buttonMapping = button_mappings_1;
    if (freePlayFlag == ModeFlag::NONE) {
        input = PlayerInputArr();
        buttonMapping = button_mappings_2;
    }

    if (!(static_cast<int>(input->PrevInputPress) & buttonMapping->PauseKey) || (*is_past_menu != 1)) {
        input = input_struct_2;
        buttonMapping = button_mappings_1;
        if (freePlayFlag == ModeFlag::NONE) {
            input = PlayerInputArr();
            buttonMapping = button_mappings_2;
        }
        ButtonMapping* puVar2 = buttonMapping + 1;
        if (!(static_cast<int32_t>(input[1].PrevInputPress) & puVar2->PauseKey) ||
            *unknown_struct_field != 1 && NeitherPressed(Input::RIGHT_FACE)) {
            if (EitherPressed(Input::BOTTOM_FACE)) {
                Native()->PlayCommonSoundEffect(SE_ACCEPT);
                switch(selection) {
                    case 0:
                        *PauseState() = 0;
                        break;
                    case 1:
                        create_fiber(KSET_controller_settings_fiber, 0x2000, 2, 1, "KSET");
                        *sub_menu_is_open = 1;
                        break;
                    case 2:
                        create_fiber(KBST_controller_settings_fiber, 0x2000, 2, 1, "KBST");
                        *sub_menu_is_open = 1;
                        break;
                    case 3:
                        create_fiber(INSD_controller_settings_fiber, 0x2000, 2, 1, "INSD");
                        *sub_menu_is_open = (check_p1_or_p2_for_input(INPUT_BOTTOM_FACE) != 1) + 1;
                        break;
                    case 4:
                        create_help_and_options_fiber();
                        break;
                    case 5:
                        create_fiber(reinterpret_cast<void*>(ModMenu), 0x2000, 1, 0x12442, "MOD_MENU");
                        *sub_menu_is_open = (check_p1_or_p2_for_input(INPUT_BOTTOM_FACE) != 1) + 1;
                        break;
                    case 6:
                        *disable_primary_menu = 1;
                        *display_confirm_exit_dialog = (check_p1_or_p2_for_input(INPUT_BOTTOM_FACE) != 1) + 1;
                        break;
                }
            }
            goto switchDefault;
        }
    }
    Native()->PlayCommonSoundEffect(SE_EXIT);
    *PauseState() = 0;

    switchDefault:
    for (int i = 0; i < itemsLength; i++) {
        Native()->RenderMenuText(
            (i == 5) ? items[i].Label : get_string(Locale(), strIds[i]),
            items[i].xOffset,
            items[i].yOffset,
            2.0f,
            255.0f / (selection == i ? 255.0f : 159.0f),
            nullptr,
            0,
            0,
            -1);
    }

    return selection;
}

/**
 *  Replaces a `does_fiber_exist` call to additionally, set the ESI register to a particular value.
 *      Coupled with a few NOP instructions, this will give control over the pause UI background size.
 */
int32_t __stdcall check_hlop_exists_wrapper() {
    bool result = does_fiber_exist("HLOP");
    if (!result) return result;

    int size = 0x60;
    if (ModMenuFiberExists()) {
        size = 0xF0;
    }

    asm(
        "movl %[aSize], %%esi"
        : // no output
        : [aSize] "g" (size)
    );

    return result;
}

void InstallModMenu_ToArcadeModeMenu() {
    void* injectAddress = reinterpret_cast<void*>(base() + offsets::UPDATE_GENERIC_PAUSE_MENU_CALL + 1);
    intptr_t hookAddress = reinterpret_cast<intptr_t>(&update_generic_pause_menu_substitute);
    
    // CallsiteAddr + instructionSize + relJumpOffset = FuncAddress
    // relJumpOffset = FuncAddress - CallsiteAddr - instructionSize
    intptr_t relativeJump = hookAddress - reinterpret_cast<intptr_t>(injectAddress) - sizeof(intptr_t);

    DWORD oldProtect;
    WINBOOL success = VirtualProtect(injectAddress, sizeof(intptr_t), PAGE_EXECUTE_READWRITE, &oldProtect);

    //memcpy(overWrittenBytes, address, size);
    memcpy(injectAddress, &relativeJump, sizeof(intptr_t));

    success = VirtualProtect(injectAddress, sizeof(intptr_t), oldProtect, &oldProtect);
}
void InstallModMenu() {
    // Mod menu
    intptr_t payload = reinterpret_cast<intptr_t>(&HLOP_fiber_entry_replacement);
    Patch(
        reinterpret_cast<void*>(base() + offsets::PUSH_HLOP_FN_INSTRUCTION + 1),
        &payload,
        sizeof(payload),
        nullptr
    );
    
    // Pause background size controller
    void* jmpDest = reinterpret_cast<void*>(&check_hlop_exists_wrapper);
    Patch_RelativeJump(
        reinterpret_cast<void*>(base() + offsets::BG_SIZE_CONTROL_DOES_FIBER_EXIST_FN + 1),
        jmpDest,
        nullptr
    );

    uint8_t nops[5] = {0x90,0x90,0x90,0x90,0x90};
    Patch(
        reinterpret_cast<void*>(base() + offsets::BG_SIZE_CONTORL_NOP_INSTRUCTIONS),
        nops,
        sizeof(nops),
        nullptr
    );
}


/////////
// API //
/////////

const BaseMod_MenuDimensions* BASEMOD_CALL GetDrawableAreaDimensions() {
    static const BaseMod_MenuDimensions _dim = {0, 85, 640, 480};
    return &_dim;
}
uint32_t BASEMOD_CALL RegisterMenuTab(const char* title, const BaseMod_ModMenuEntry* entries, uint32_t numEntries) {
    _modMenuTabs.emplace_back(MenuTab{title, entries, numEntries, nullptr});
    return 0;
}
uint32_t BASEMOD_CALL RegisterCustomMenuTab(const char* title, BM_CustomMenuHandler handler) {
    _modMenuTabs.emplace_back(MenuTab{title, nullptr, 0, handler});
    return 0;
}

const BaseMod_ModMenu_HelperFunctionsApi* GetHelperFunctionApi() {
    static const BaseMod_ModMenu_HelperFunctionsApi _api = {
        sizeof(BaseMod_ModMenu_HelperFunctionsApi),
        BASEMOD_API_VERSION_NUM,
        HandleMenuSelection,
        HoldDirectionInputHandler,
        DrawEnumSettingUI,
        DrawGaugeSettingUI,
        DrawScrollArrow,
        SwitchToMainFiber,
    };
    return &_api;
}
const BaseMod_ModMenuApi* GetModMenuApi() {
    static const BaseMod_ModMenuApi _api = {
        sizeof(BaseMod_ModMenuApi),
        BASEMOD_API_VERSION_NUM,
        GetHelperFunctionApi(),
        GetDrawableAreaDimensions,
        RegisterMenuTab,
        RegisterCustomMenuTab,
    };
    return &_api;
}
