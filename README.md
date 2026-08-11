# GearLoader - Mod Loader for GGXXACPR (Steam)
Based on the original [Gear-DLL-Loader](https://github.com/MoonSquaredd/Gear-DLL-Loader) by [lovenus](https://github.com/MoonSquaredd)


GearLoader: v0.1.0 <br>
baseMod: v1.1.1

GearLoader is a DLL-based mod loader and manager for the Steam release of Guilty Gear Accent Core Plus R. GearLoader comes bundled with baseMod, a modding API to support mod development.


## Install Instructions
Grab the latest "GearLoader.zip" from [releases](https://github.com/YouKnow232/GearLoader/releases), unpack it and place
its contents in your game folder where GGXXACPR_Win.exe is.
Any mod that requires it should be placed in the mods folder.

```text
Guilty Gear XX Accent Core Plus R/
├── GGXXACPR_Win.exe
├── DBGHELP.dll
└── mods/
    └── baseMod/
```

When installing on Linux/Steam Deck, add `WINEDLLOVERRIDES="dbghelp=n,b" %command%` to the game's launch options.

Additional launch options include:
* `-GearLoaderVerbose` for verbose logging to `GearLoader.log`
* `-DebugConsole` to open a console window on launch


## Making a Mod
See the tutorial and documentation [here!](https://youknow232.github.io/GearLoader/doxygen/md_docs_2_modding_instructions.html)

### Including GearLoader via CMake
The include headers from GearLoader can be made available through CMake using either FetchContent for add_subdirectory. Make sure to set the `GEARLOADER_INCLUDE_ONLY` variable to `ON` to avoid adding the entire project. When specifying the `GIT_TAG`, you can use version tags after v.1.1.1 (e.g. `GIT_TAG v1.2.0`). CMake support was added after v1.1.1's initial release so you'll need to use a slightly different tag for that version: `GIT_TAG v1.1.1-CMake`.

Example using FetchContent:
```cmake
set(GEARLOADER_INCLUDE_ONLY ON)
FetchContent_Declare(
    GearLoader
    GIT_REPOSITORY https://github.com/youknow232/GearLoader.git
    GIT_TAG v1.1.1-CMake
)
FetchContent_MakeAvailable(GearLoader)

# ...

target_link_libraries(MyBuildTarget PUBLIC
    GearLoader::GearLoader
)
```

Source example:
```C
#include gearLoader/gearLoader_c.h
// C++ alternative:
// #include gearLoader/gearLoader.hpp

GEARLOADER_EXPORT void GEARLOADER_CALL Init(GearLoaderContext* ctx, GearLoaderApi* api) {
    // Init code goes here
}
```

## Build Instructions
### Prerequisites
* CMake v3.21 or later
* gcc v15.2.0 or later
  * i686-w64-mingw32-g++
  * i686-w64-mingw32-gcc
* An internet connection

For Windows users:
* MSYS2 environment (http://msys2.org)
* In a MINGW32 shell, make sure you have access to gcc and cmake


### Instructions
Run the `Compile.sh` script in the project root directory (from a MINGW32 shell for Windows users).
This may take a minute or two the first time as CMake fetches dependencies.

Alternatively, you can manually configure and build via CMake.
If cross-compiling from Linux/iOS use the `toolchain.cmake` file
when configuring the build files.


### Troubleshooting
If you are getting assembler errors, a vague reference to `nativeFunctions.cpp`, and are using a compiler other than gcc, the issue is likely from incompatibilities in the asm declarations in `nativeFunctions.cpp`. Either use gcc or you can try adapting the short asm declarations to your compiler of choice.
