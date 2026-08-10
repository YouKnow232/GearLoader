cmake_minimum_required(VERSION 3.21)
project(GearLoader
    VERSION 2.0.0
    DESCRIPTION "Mod Loader for GGXXACPR"
    LANGUAGES
        CXX
        C
)

option(GEARLOADER_INCLUDE_ONLY "Only create the GearLoader includes interface" OFF)

if (NOT DEFINED GEARLOADER_TARGET_NAME)
    # Allow overriding the target name when using FetchContent / add_subdirectory.
    set(GEARLOADER_TARGET_NAME ${PROJECT_NAME})
endif()

## create exportable include target
add_library(${GEARLOADER_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${GEARLOADER_TARGET_NAME} ALIAS ${GEARLOADER_TARGET_NAME})
target_include_directories(GearLoader INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# Main cmake project
if (NOT GEARLOADER_INCLUDE_ONLY)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

    add_subdirectory(examples/c_mod)
    add_subdirectory(examples/cpp_mod)
    add_subdirectory(source/baseMod)
    add_subdirectory(test/baseModTester)

    # Dependencies
    include(FetchContent)

    set(JSON_MultipleHeaders OFF)
    # set(SPDLOG_BUILD_SHARED ON)

    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG 55f93686c01528224f448c19128836e7df245f72 # v3.12.0
    )
    FetchContent_Declare(
        gabime_spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 79524ddd08a4ec981b7fea76afd08ee05f83755d # v1.17.0
    )

    FetchContent_MakeAvailable(
        nlohmann_json
        gabime_spdlog
    )

    # util object files (Used in both main DLL and testing suite)
    add_library(core STATIC)
    target_sources(core
        PRIVATE
            source/modLoader/common/versionParsing.cpp
            source/modLoader/dependencyManager/configParser.cpp
            source/modLoader/dependencyManager/dependencyManager.cpp
            source/modLoader/gearLoaderApi/gearLoader.cpp
    )
    # Dependencies
    target_link_libraries(core PUBLIC
        nlohmann_json::nlohmann_json
        spdlog::spdlog
    )
    target_include_directories(core PRIVATE
        include
        source/modLoader
    )

    # Main mod loader dll
    add_library(GearLoaderDll SHARED
        source/modLoader/source.cpp
        source/modLoader/exports.def
    )
    target_include_directories(GearLoaderDll PRIVATE
        include
        source/modLoader
    )
    target_link_libraries(GearLoaderDll PRIVATE core)
    # target_sources(GearLoaderDll PRIVATE
    #     $<TARGET_OBJECTS:core>
    # )
    target_compile_definitions(GearLoaderDll
        PRIVATE exports
    )
    set_target_properties(GearLoaderDll PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/GearLoader
        PREFIX ""
        OUTPUT_NAME "DBGHELP"
    )
    target_link_options(GearLoaderDll PRIVATE
        -static
    )

    # Unit testing exe
    add_executable(Tests
        test/test.cpp
    )
    target_include_directories(Tests PRIVATE
        include
        source/modLoader
    )
    target_link_libraries(Tests PRIVATE core)
    # target_sources(Tests PRIVATE
    #     $<TARGET_OBJECTS:core>
    # )
    set_target_properties(Tests PROPERTIES
        OUTPUT_NAME "RunTests"
    )
    target_link_options(Tests PRIVATE
        -static
    )

    add_custom_command(
        OUTPUT ${BIN_DIR}/GearLoader/README.txt
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}/GearLoader
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/installation-README.txt
            ${BIN_DIR}/GearLoader/README.txt
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docs/installation-README.txt
        COMMENT "Copying install readme"
    )
    add_custom_target(copy_install_readme ALL
        DEPENDS ${BIN_DIR}/GearLoader/README.txt
    )

# end include only clause
endif()