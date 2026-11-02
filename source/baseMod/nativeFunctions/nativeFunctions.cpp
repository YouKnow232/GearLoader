#include "nativeFunctions.h"
#include <cstdint>
#include "baseMod_p.h"
#include "offsets.h"

/**
 *  Native Functions
 * 
 *  The internal functions of the game code are compiled to non-standard calling conventions
 *      and require hand-written assembly to invoke. The functions written in this file
 *      are minimal wrapper functions that only exist to adapt the calling convention.
 * 
 *  See `BaseMod_NativeFunctionsApi` for documentation on the native functions themselves.
 * 
 *  Extended asm declaration documentation:
 *  https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 *  Specific register constraints (x86 family):
 *  https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
 */

using NativeRenderCockpitFontText = void (__stdcall *)(int32_t xPos, int32_t yPos, float zPos, uint8_t alpha, float size);
static NativeRenderCockpitFontText _nativeRenderCockpitFontText =
    reinterpret_cast<NativeRenderCockpitFontText>(getBaseAddress() + offsets::RENDER_COCKPIT_FONT_TEXT_FN);

void BASEMOD_CALL RenderCockpitFontText(
    const char* text,
    int32_t xPos,
    int32_t yPos,
    float zPos,
    uint8_t alpha,
    float size) noexcept
{
    asm (
        "push %[aSize]\n\t"
        "push %[aAlpha]\n\t"
        "push %[aZPos]\n\t"
        "push %[aYPos]\n\t"
        "push %[aXPos]\n\t"
        "call *%[fn]\n\t"
        "addl $24, %%esp"
        : // no output
        : [fn] "g" (_nativeRenderCockpitFontText),
          [aXPos] "g" (xPos),
          [aYPos] "g" (yPos),
          [aZPos] "g" (zPos),
          [aAlpha] "g" (alpha),
          [aSize] "g" (size),
          "c" (text) // ECX
        : "memory", "cc" // clobbered
    );
}

void BASEMOD_CALL RenderMenuText(
    const char* text,
    int32_t xPos,
    int32_t yPos,
    float zPos,
    float alpha,
    int32_t* animCounter,
    int32_t animSpeed,
    int32_t ignoreSpriteMask,
    uint32_t color
) {
    asm(
        "push %[aIgnoreSpriteMask]\n\t"
        "push %[aSpeed]\n\t"
        "push %[aCounter]\n\t"
        "push %[aAlpha]\n\t"
        "push %[aZPos]\n\t"
        "push %[aYPos]\n\t"
        "push %[aXPos]\n\t"
        "push %[aText]\n\t"
        "call *%[func]\n\t"
        "addl $32, %%esp"
        : // no output
        : [func] "g" (getBaseAddress() + offsets::DRAW_MENU_TEXT_FN),
          [aText] "g" (text),
          [aXPos] "g" (xPos),
          [aYPos] "g" (yPos),
          [aZPos] "g" (zPos),
          [aAlpha] "g" (alpha),
          [aCounter] "g" (animCounter),
          [aSpeed] "g" (animSpeed),
          [aIgnoreSpriteMask] "g" (ignoreSpriteMask),
          "a" (color)  // eax
        : "memory", "cc"
    );
}

void BASEMOD_CALL RenderMenuTextCenterAligned(
    const char* text,
    int32_t xPos,
    int32_t yPos,
    float zPos,
    float alpha,
    uint32_t color,
    int32_t ignoreSpriteMask
) {
    asm(
        "push %[aIgnoreSpriteMask]\n\t"
        "push %[aColor]\n\t"
        "push %[aAlpha]\n\t"
        "push %[aZPos]\n\t"
        "push %[aYPos]\n\t"
        "push %[aXPos]\n\t"
        "call *%[func]\n\t"
        "addl $32, %%esp"
        : // no output
        : [func] "g" (getBaseAddress() + offsets::DRAW_MENU_TEXT_CENTERED_FN),
          [aXPos] "g" (xPos),
          [aYPos] "g" (yPos),
          [aZPos] "g" (zPos),
          [aAlpha] "g" (alpha),
          [aColor] "g" (color),
          [aIgnoreSpriteMask] "g" (ignoreSpriteMask),
          "a" (text)  // eax
        : "memory", "cc"
    );
}

using NativeRenderPopUpText = void (__stdcall *)();
static NativeRenderPopUpText _nativeRenderPopUpText =
    reinterpret_cast<NativeRenderPopUpText>(getBaseAddress() + offsets::RENDER_POPUP_TEXT_FN);

void BASEMOD_CALL RenderPopUpText(int playerIndex, const char* text) {
    asm (
        "call *%[fn]"
        : // no output
        : [fn] "g" (_nativeRenderPopUpText),
          "a" (playerIndex),    // EAX
          "S" (text)            // ESI
        : "cc" // clobbered
    );
}

uint32_t BASEMOD_CALL PlayCommonSoundEffect(uint32_t id) {
    uint32_t output;
    asm(
        "call *%[fn]"
        : "=a" (output)
        : [fn] "g" (getBaseAddress() + offsets::PLAY_SOUND_EFFECT_FN),
          "S" (id)  // ESI
        : "cc" // clobbered
    );
    return output;
}

void BASEMOD_CALL DrawSprite(GGXXACPR_DrawSpriteParams* params, int32_t ignoreMask) {
    asm(
        "push %[aIgnoreMask]\n\t"
        "call *%[fn]\n\t"
        "addl $4, %%esp"
        : // no output
        : [fn] "g" (getBaseAddress() + offsets::DRAW_SPRITE_FN),
          [aIgnoreMask] "g" (ignoreMask),
          "c" (params) // ECX
        : "memory", "cc" // clobbered
    );
}

void BASEMOD_CALL DrawTriStrip(GGXXACPR_ColorVertex* vertices, uint32_t numVertices) {
    asm(
        "push %[aVertices]\n\t"
        "call *%[fn]\n\t"
        "addl $4, %%esp"
        : // no output
        : [fn] "g" (getBaseAddress() + offsets::DRAW_TRI_STRIP_FN),
          [aVertices] "g" (vertices),
          "D" (numVertices) // EDI
        : "memory", "cc" // clobbered
    );
};

using NativeDrawQuad = void (__stdcall *)(int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t zPos, uint32_t color);
void BASEMOD_CALL DrawQuad(int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t zPos, uint32_t color) {
    static NativeDrawQuad _native = reinterpret_cast<NativeDrawQuad>(getBaseAddress() + offsets::DRAW_QUAD_FN);
    _native(left, top, right, bottom, zPos, color);
}

// Menu Functions

using draw_menu_arrow_t = void (__stdcall*)(uint32_t, int32_t, int32_t, int32_t, int32_t);
// see `BM_DrawArrowSpriteDirection` for directionFlag param
void BASEMOD_CALL DrawArrowSprite(uint32_t directionFlag, int32_t x, int32_t y, int32_t z, uint32_t alpha) {
    reinterpret_cast<draw_menu_arrow_t>(getBaseAddress() + offsets::DRAW_MENU_ARROW_FN)(
        directionFlag, x, y, z, alpha
    );
}

void BASEMOD_CALL DrawGaugeSettingUI(int32_t y, Native_MenuEntry* entry, uint8_t alpha, int32_t maxValue) {
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
          "c" (y) // ECX
        : "memory", "cc" // clobbered
    );
}

uint32_t BASEMOD_CALL RegisterSprites(uint32_t spriteId, void* textureDataArray, uint32_t count) {
    uint32_t result;
    asm(
        "push %[aCount]\n\t"
        "push %[aArray]\n\t"
        "call *%[fn]\n\t"
        "addl $12, %%esp"
        : "=a" (result)
        : [fn] "g" (getBaseAddress() + offsets::REGISTER_TEXTURE_DATA_FN)
        , [aCount] "g" (count)
        , [aArray] "g" (textureDataArray)
        , "a" (spriteId) // EAX
        : "memory", "cc"
    );
    return result;
}

uint32_t BASEMOD_CALL GetActiveCommandGrabId(GGXXACPR_Entity* player) {
    uint32_t result;
    asm(
        "call *%[fn]\n\t"
        "movl %%eax, %[aResult]"
        : [aResult] "=g" (result)
        : [fn] "g" (getBaseAddress() + offsets::GET_ACTIVE_COMMAND_GRAB_ID_FN)
        , "a" (player) // EAX
        : "memory", "cc"
    );
    return result;
}

int32_t BASEMOD_CALL DoesFiberExist(const char* name) {
    int32_t output;
    asm(
        "call *%[fn]"
        : "=a" (output)
        : [fn] "r" (getBaseAddress() + offsets::DOES_FIBER_EXIST_FN),
          "a" (name)
        : "cc" // clobbered
    );
    return output;
}


const BaseMod_NativeFunctionsApi* GetNativeFunctionsApi() {
    static const BaseMod_NativeFunctionsApi _api = {
        size: sizeof(BaseMod_NativeFunctionsApi),
        version: BASEMOD_API_VERSION_NUM,
        // v0.1.0
        RenderCockpitFontText: RenderCockpitFontText,
        // v1.0.0
        RenderMenuText: RenderMenuText,
        RenderMenuTextCenterAligned: RenderMenuTextCenterAligned,
        RenderPopUpText: RenderPopUpText,
        PlayCommonSoundEffect: PlayCommonSoundEffect,
        DrawSprite: DrawSprite,
        DrawArrowSprite: DrawArrowSprite,
        DrawTriStrip: DrawTriStrip,
        DrawQuad: DrawQuad,
        // v1.1.0
        RegisterSprites: RegisterSprites,
        GetActiveCommandGrabId: GetActiveCommandGrabId,
        // v1.2.0
        DoesFiberExist: DoesFiberExist,
    };

    return &_api;
}
