cmake_minimum_required(VERSION 3.21)


add_library(baseMod SHARED
    baseMod.cpp
    gameData/gameData.cpp
    gameData/charData/charData.cpp
    hookManager/hookManager.cpp
    modMenu/modMenu.cpp
    nativeFunctions/nativeFunctions.cpp
    resourceRedirector/resourceRedirector.cpp
)
target_include_directories(baseMod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
)
set_target_properties(baseMod PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/GearLoader/mods/baseMod
    PREFIX ""
    OUTPUT_NAME "baseMod"
)
target_link_options(baseMod PRIVATE
    -static
)

add_custom_command(
    OUTPUT ${BIN_DIR}/GearLoader/mods/baseMod/config.json
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${BIN_DIR}/GearLoader/mods/baseMod/config.json
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.json
    COMMENT "Copying baseMod config.json"
)
add_custom_target(copy_baseMod_config ALL
    DEPENDS ${BIN_DIR}/GearLoader/mods/baseMod/config.json
)
