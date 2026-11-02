#pragma once

#include "baseMod_p.h"
#include <cstdint>
#include "gearLoader/ggxxacpr_c.h"
#include "offsets.h"

const BaseMod_NativeFunctionsApi* GetNativeFunctionsApi();

// Internal only:
// TODO refactor to use public api version
inline bool does_fiber_exist(const char* name) {
    uint32_t output;
    asm(
        "call *%[fn]"
        : "=a" (output)
        : [fn] "r" (getBaseAddress() + offsets::DOES_FIBER_EXIST_FN),
          "a" (name)
        : "cc" // clobbered
    );
    return output;
}
// create_fiber
using create_fiber_t = GGXXACPR_FiberData* (__stdcall*)(
    void* entry_fiber,
    int size,
    int param_3,
    int param_4,
    const char* name);
inline GGXXACPR_FiberData* create_fiber(
    void* entry_fiber,
    int size,
    int param_3,
    int param_4,
    const char* name
) {
    return reinterpret_cast<create_fiber_t>(getBaseAddress() + offsets::CREATE_FIBER_FUNC_OFFSET)(
        entry_fiber, size, param_3, param_4, name);
}

inline uint32_t check_p1_or_p2_for_input(GGXXACPR_RawControllerInput input) {
    uint32_t output;
    asm(
        "call *%[fn]"
        : "=a" (output)
        : [fn] "r" (getBaseAddress() + offsets::CHECK_INPUT_P1DOP2P_FUNC_OFFSET),
          "c" (input)   // ECX
        : "cc" // clobbered
    );
    return output;
}
// create_help_and_options_fiber
inline void create_help_and_options_fiber() {
    reinterpret_cast<void (__stdcall*)()>(getBaseAddress() + offsets::CREATE_HELP_AND_OPTIONS_FIBER_FUNC_OFFSET)();
}
// get_string
inline const char* get_string(GGXXACPR_LocaleState* dictionary, int id) {
    const char* output;
    asm(
        "call *%[fn]"
        : "=a" (output)
        : [fn] "r" (getBaseAddress() + offsets::GET_STRING_FUNC_OFFSET),
          "a" (dictionary), // EAX
          "d" (id)          // EDX
        : "cc" // clobbered
    );
    return output;
}
using draw_menu_header_font_t = void (__stdcall*)(const char*, float, float, float, uint32_t, uint32_t, float);
inline void draw_menu_header_font(
    const char* text,
    float x,
    float y,
    float z,
    uint32_t alpha,
    uint32_t fontId,
    float size
) {
    reinterpret_cast<draw_menu_header_font_t>(
        getBaseAddress() + offsets::DRAW_MENU_HEADER_FONT_FUNC
        )(text, x, y, z, alpha, fontId, size);
}

struct Native_MenuEntry {
    const char* Label;
    int32_t xOffset;
    int32_t yOffset;
    int32_t Value;
    uint32_t NumValues;
    const char** ValueLabels;
};
inline void draw_gauge_setting_ui(int32_t yPos, Native_MenuEntry* entry, uint8_t alpha, int32_t maxValue) {
    asm(
        "push %[aMaxValue]\n\t"
        "push %[aAlpha]\n\t"
        "push %[aEntry]\n\t"
        "call *%[fn]\n\t"
        "addl $12, %%esp"
        : // no output
        : [fn] "r" (getBaseAddress() + offsets::DRAW_GAUGE_SETTING_UI_FN),
          [aEntry] "g" (entry),
          [aAlpha] "g" (alpha),
          [aMaxValue] "g" (maxValue),
          "c" (yPos) // ECX
        : "memory", "cc" // clobbered
    );
}

inline void modify_string(char* buffer) {
    asm(
        "call *%[fn]"
        : // no output
        : [fn] "r" (getBaseAddress() + offsets::MODIFY_STRING_FN),
           "c" (buffer) // ECX
        : "memory"
    );
}

inline void lff_call(uint32_t sprite_id, void* sprite_buffer, int terminating_value) {
    asm(
        "push %[aSprBuf]\n\t"
        "push %[aTermVal]\n\t"
        "call *%[fn]\n\t"
        "addl $8, %%esp"
        : // no output
        : [fn] "r" (getBaseAddress() + offsets::LFF_CALL_FN),
          [aSprBuf] "g" (sprite_buffer),
          [aTermVal] "g" (terminating_value),
          "a" (sprite_id) // EAX
        : "memory", "cc"
    );
}