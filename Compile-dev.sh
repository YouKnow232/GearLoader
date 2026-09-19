#!/bin/bash

# This script expects to be run in the project root directory
# For Windows environments, a MINGW32 shell is expected

set -e

BUILD_DIR="build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"


if [ ! -f "CMakeCache.txt" ]; then
    echo "Running CMake configuration.."

    if [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32"* ]]; then
        cmake -G "MinGW Makefiles" ..
    else
        cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
    fi
else
    echo "build directory already configured."
fi

cmake --build .

echo # line break

echo == Running Tests ==
cd ..

if [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32"* ]]; then
    ./bin/RunTests.exe
else
    echo "TODO build tests for linux"
fi


if [[ -d "./bin/GearLoader" && ! -z "$GGXXACPR_DIR" ]]; then
    echo # line break
    echo "Copying files:"
    cp -ru --verbose "./bin/GearLoader/mods" "./bin/GearLoader/DBGHELP.dll" "$GGXXACPR_DIR"
    cp -ru --verbose "./bin/baseModTester" "$GGXXACPR_DIR/mods"
fi
