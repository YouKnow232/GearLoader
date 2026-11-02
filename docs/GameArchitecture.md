# Game Architecture Notes

## Inspecting Game Code with Ghidra
See the Ghidra file in the [moddingResources](https://github.com/YouKnow232/GearLoader/tree/main/moddingResources) folder. Offset references in this article are references to specific points in the compiled game code (ex. +0x221ed0). Ghidra by default mounts the game code at base address 0x00400000. If you don't change it, Ghidra will show the example offset as the absolute address of 0x00621ed0. You can change the base address via Window -> Memory map -> Set Image Base (House icon)

In Ghidra, some labels of note include `main`, `main_game_loop`, and `update_players`.


## Fibers
GGXXACPR uses [Fibers](https://learn.microsoft.com/en-us/windows/win32/procthread/fibers) to handle asynchronous code. When the game boots it converts it's main thread into a fiber with `ConvertThreadToFiber` (+0x10a046). New fibers are created through the `create_fiber` function. Fibers and their state data are managed in the `named_fiber_array`. From within a fiber, calling the `winnt.h` macro `GetFiberData` will return a pointer to the current fiber's entry in the `named_fiber_array`.

Some example fibers:

| Name     | Description
| ---      | ---
| OPTION   | Main menu fiber
| MMV2     | Handles netplay matchmaking. You can generally check for this to detect netplay
| HLOP     | Help and Options menu fiber. (gets modified by BaseMod to include "Mod Settings" option)
| MOD_MENU | Custom fiber created by BaseMod
