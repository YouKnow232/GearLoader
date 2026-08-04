#pragma once

#include <cstdint>

#define OFFSETS_FOR_STEAM_BUILD_ID "12866140"

namespace offsets {

constexpr intptr_t IN_GAME_FLAG = 0x7101F4;
constexpr intptr_t GAME_VER_FLAG = 0x6D0538;    // 0=AC, 1=+R
constexpr intptr_t MAIN_MENU_SELECTION = 0x70FFB4;
constexpr intptr_t GAME_MODE_FEATURE_FLAGS = 0x6D64CC;

// Injection Addresses
constexpr intptr_t MESSAGE_LOOP_END = 0x222413;
constexpr intptr_t PRESENT_CALL_INSTRUCTION = 0x2271D8;
constexpr intptr_t GRAPHICS_HOOK_BREAKPOINT = 0x2271DA;
constexpr intptr_t PEEK_MESSAGE_FUNCTION_POINTER = 0x3BD348;
constexpr intptr_t MESSAGE_LOOP_REL_JMP_OFFSET_BYTE_ADDR = 0x222414;
constexpr intptr_t GRAPHICS_HOOK_TARGET_FUNCTION_CALL = 0x22717A;
constexpr intptr_t GRAPHICS_HOOK_TARGET_FUNCTION_ADDRESS = 0x2227E0;
constexpr intptr_t BEFORE_PRESENT_DEVICE_REFERENCE = 0x2271C5;
constexpr intptr_t UPDATE_GAME_STATE_RET_INSTRUCTION = 0x3A1B78;
constexpr intptr_t COMMON_SIM_UPDATE_FUNCTION_CALL = 0x3A199E;
constexpr intptr_t SET_GRAPHICS_CONTEXT_CALL = 0x227109;
constexpr intptr_t PUSH_HLOP_FN_INSTRUCTION = 0x356ADF;
constexpr intptr_t SAVE_GAME_DATA_FN = 0x3662B0;
constexpr intptr_t SAVE_GAME_DATA_FN_CALL = 0x2C3BF7;

constexpr intptr_t INIT_THREAD_AND_DEVICES_FUNC = 0x221720;
constexpr intptr_t INIT_THREAD_AND_DEVICES_CALL = 0x2223A3;

// Native function address
constexpr intptr_t RENDER_COCKPIT_FONT_TEXT_FN = 0x1E9610;
constexpr intptr_t COMMON_SIM_UPDATE_FUNC = 0x1BD880;
constexpr intptr_t SET_GRAPHICS_CONTEXT_FUNC = 0x225C40;
constexpr intptr_t RENDER_POPUP_TEXT_FN = 0x1E8F50;
constexpr intptr_t DRAW_TRI_STRIP_FN = 0x107620;
constexpr intptr_t DRAW_TRI_STRIP_2_FN = 0x1077D0;
constexpr intptr_t DRAW_QUAD_FN = 0x38DA40;
constexpr intptr_t DRAW_SPRITE_FN = 0x10D1A0;
constexpr intptr_t REGISTER_TEXTURE_DATA_FN = 0x113820;
constexpr intptr_t GET_ACTIVE_COMMAND_GRAB_ID_FN = 0x1201D0;

// Hack Addresses
constexpr intptr_t FIX_BACKGROUND_STATE_INSTRUCTION = 0x21C363;
constexpr intptr_t BACKGROUND_STATE = 0x6D6420;  // see enum BackgroundState

// DirectX
constexpr intptr_t DIRECT3D9_DEVICE = 0x710580;

// Entities
constexpr intptr_t PLAYER_1_PTR = 0x6D1378;
constexpr intptr_t PLAYER_2_PTR = 0x6D4C84;
constexpr intptr_t ROOT_ENTITY = 0x6D27A8;
constexpr intptr_t ENTITY_LIST_PTR = 0x6D137C;

// Player Data
constexpr intptr_t PLAYER_INPUT = 0x6D0E80;

// Camera
constexpr intptr_t CAMERA = 0x6D5CD0;
constexpr intptr_t VIEW_HEIGHT = 0x6C118C;
constexpr intptr_t VIEW_WIDTH = 0x6C14E4;
constexpr intptr_t WINDOW_MODE = 0x6C1510; // 0=Window 1=Full 2=Borderless

// Pushboxes
constexpr intptr_t PUSHBOX_STANDING_WIDTH_ARRAY = 0x571564;
constexpr intptr_t PUSHBOX_STANDING_HEIGHT_ARRAY_AC = 0x571784;
constexpr intptr_t PUSHBOX_STANDING_HEIGHT_ARRAY_PR = 0x571E6C;
constexpr intptr_t PUSHBOX_CROUCHING_WIDTH_ARRAY = 0x573154;
constexpr intptr_t PUSHBOX_CROUCHING_HEIGHT_ARRAY = 0x573B38;
constexpr intptr_t PUSHBOX_AIR_WIDTH_ARRAY = 0x573B6C;
constexpr intptr_t PUSHBOX_AIR_HEIGHT_ARRAY = 0x573BA0;
constexpr intptr_t PUSHBOX_AIR_OFFSET_ARRAY_AC = 0x573BD4;
constexpr intptr_t PUSHBOX_AIR_OFFSET_ARRAY_PR = 0x573C08;
// Y offset values for Airborne pushboxes (Almost always equal to abs(YPos)+4000 except for Kliff)
constexpr intptr_t PUSHBOX_P1_JUMP_OFFSET = 0x6D6378;
constexpr intptr_t PUSHBOX_P2_JUMP_OFFSET = 0x6D637C;
constexpr intptr_t PUSHBOX_EDGE_DISTANCE = 0x6D638C;

// Throws
constexpr intptr_t PLUSR_GROUND_THROW_RANGE_ARRAY = 0x57005C;
constexpr intptr_t AC_GROUND_THROW_RANGE_ARRAY = 0x56FF6C;
constexpr intptr_t PLUSR_AIR_THROW_HORIZONTAL_RANGE_ARRAY = 0x5708DC;
constexpr intptr_t AC_AIR_THROW_HORIZONTAL_RANGE_ARRAY = 0x570174;
constexpr intptr_t AIR_THROW_LOWER_RANGE_ARRAY = 0x5709B4;
constexpr intptr_t AIR_THROW_UPPER_RANGE_ARRAY = 0x570A8C;
constexpr intptr_t COMMAND_GRAB_ID_P1 = 0x6D6384;
constexpr intptr_t COMMAND_GRAB_ID_P2 = 0x6D6388;
constexpr intptr_t COMMAND_GRAB_RANGE_LOOKUP_TABLE = 0x572110;
// one byte [P1Throwable, P2Throwable, P1ThrowActive P2ThrowActive]
constexpr intptr_t GLOBAL_THROW_FLAGS = 0x6D5D7C;

// Pause Menus
// 0 = not paused, 1 or 2 = paused (not sure the difference between 1 and 2)
constexpr intptr_t PAUSE_STATE = 0x7109E4;
constexpr intptr_t PAUSE_DISPLAY_STATE = 0x6CBD20;

// Replay
// 1 = normal, 0 = do not simulate, -1 = rewinding (stays at 0 for frame stepping)
constexpr intptr_t GLOBAL_REPLAY_SIMULATE = 0x7D5788;
constexpr intptr_t REPLAY_FRAME_COUNT = 0x7D57D8;

// Program flow
constexpr intptr_t JOB_MODE = 0x7109EC;

// Survival Mode
constexpr intptr_t SURVIVAL_SHADOW_BOSS_CHECK = 0x5F9070;
constexpr intptr_t SURVIVAL_GOLDEN_BOSS_CHECK = 0x5F9264;
constexpr intptr_t SURVIVAL_UPGRADE_WEIGHTS = 0x5F94D8;
constexpr intptr_t SURVIVAL_ATTACKLV_MAX = 0x5F9534;
constexpr intptr_t SURVIVAL_DEFENSELV_MAX = 0x5F9538;
constexpr intptr_t SURVIVAL_SPEEDLV_MAX = 0x5F953C;
constexpr intptr_t SURVIVAL_TENSIONLV_MAX = 0x5F9540;
constexpr intptr_t SURVIVAL_STENSIONLV_MAX = 0x5F9544;
constexpr intptr_t SURVIVAL_BURSTLV_MAX = 0x5F9548;
constexpr intptr_t SURVIVAL_ADASHLV_MAX = 0x5F954C;
constexpr intptr_t SURVIVAL_AJUMPLV_MAX = 0x5F9550;
constexpr intptr_t SURVIVAL_INFO = 0x6A6F60;

// Mod Menu
// Globals
constexpr intptr_t PAUSE_MENU_SELECTION = 0x875F68;
constexpr intptr_t SUB_MENU_IS_OPEN = 0x6D0C2C;
constexpr intptr_t INPUT_STRUCT_2 = 0x6D95B8;
constexpr intptr_t PLAYER_INPUT_ARR = 0x6D9A18;
constexpr intptr_t MENU_INPUT_HOLD_TIMER = 0x6D0C20;
constexpr intptr_t BUTTON_MAPPINGS_1 = 0x69ED40;
constexpr intptr_t BUTTON_MAPPINGS_2 = 0x6CE030;
constexpr intptr_t IS_PAST_MENU = 0x6C6878;
constexpr intptr_t ASTRUCT_5_FIELD = 0x6C68B0;
constexpr intptr_t KSET_FIBER_FUNCTION = 0x350270;
constexpr intptr_t KBST_FIBER_FUNCTION = 0x352830;
constexpr intptr_t INSD_FIBER_FUNCTION = 0x2E2680;
constexpr intptr_t DISABLE_PRIMARY_MENU = 0x6CBD24;
constexpr intptr_t DISPLAY_CONFIRM_EXIT_DIALOG = 0x6CBD0C;
constexpr intptr_t CURRENT_LOCALE_MODE = 0x726570;
constexpr intptr_t STRING_DICTIONARY_UNICODE = 0x66EBE8;
constexpr intptr_t STRING_DICTIONARY = 0x684908;
constexpr intptr_t MENU_SCROLL_UP_ARROW_SPRITE = 0x6A9C28;
constexpr intptr_t MENU_SCROLL_DOWN_ARROW_SPRITE = 0x6BB778;
// Function offsets
constexpr intptr_t DOES_FIBER_EXIST_FN = 0x103100;
constexpr intptr_t PLAY_SOUND_EFFECT_FN = 0x11f820;
constexpr intptr_t CREATE_FIBER_FUNC_OFFSET = 0x1034C0;
constexpr intptr_t CHECK_INPUT_P1DOP2P_FUNC_OFFSET = 0x11C710;
constexpr intptr_t CREATE_HELP_AND_OPTIONS_FIBER_FUNC_OFFSET = 0x356AC0;
constexpr intptr_t GET_STRING_FUNC_OFFSET = 0x3186A0;
constexpr intptr_t DRAW_MENU_TEXT_FN = 0x354510;
constexpr intptr_t DRAW_MENU_TEXT_CENTERED_FN = 0x355000;
constexpr intptr_t DRAW_MENU_HEADER_FONT_FUNC = 0x115AC0;
constexpr intptr_t DRAW_MENU_ARROW_FN = 0x1156B0;
constexpr intptr_t DRAW_GAUGE_SETTING_UI_FN = 0x2BC460;
constexpr intptr_t LFF_CALL_FN = 0x113820;
// Sprite Loading
constexpr intptr_t SCROLL_ARROW_LOAD_SYMBOL = 0x6DBB88;
constexpr intptr_t TRAINING_MENU_INIT_FN = 0x2BF710;
// HLOP funcs
constexpr intptr_t CTRLS_FIBER_ENTRY = 0x3564B0;
constexpr intptr_t CTRLS_CHECK_FN = 0x355D60;
constexpr intptr_t HOWTO_FIBER_ENTRY = 0x356480;
constexpr intptr_t HOWTO_CHECK_FN = 0x355D40;
constexpr intptr_t GLSFR_FIBER_ENTRY = 0x355DC0;
constexpr intptr_t GLSFR_CHECK_FN = 0x355D20;
constexpr intptr_t GLSFR_GGPO_FIBER_ENTRY = 0x355D80;
constexpr intptr_t GLSFR_GGPO_CHECK_FN = 0x355D00;
constexpr intptr_t MODIFY_STRING_FN = 0x318610;
// BG Size Detour
constexpr intptr_t BG_SIZE_CONTROL_DOES_FIBER_EXIST_FN = 0x1C2767;
constexpr intptr_t BG_SIZE_CONTORL_NOP_INSTRUCTIONS = 0x1C2770;
constexpr intptr_t SET_BG_DETOUR_RETURN = 0x1C27E6;

// Hook
constexpr intptr_t UPDATE_GENERIC_PAUSE_MENU_CALL = 0x1C2CAD;
// Fiber
constexpr intptr_t MAIN_FIBER = 0x710C34;
constexpr intptr_t FIBER_COUNTER = 0x710C3C;
constexpr intptr_t FRAME_COUNTER = 0x6D65A8;
}
