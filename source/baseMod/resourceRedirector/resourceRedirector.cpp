#include "resourceRedirector.h"
#include <filesystem>
#include <unordered_map>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

const fs::path defaultModDir = ".\\mods\\";
const fs::path subFolder = "Resource\\";

static bool initialized = false;
static std::unordered_map<fs::path, std::string> redirectMap;


void RegisterFolderContents(const fs::path& path) {
    if (!fs::is_directory(path)) return;

    // DEBUG
    int count = 0;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            fs::path key = fs::relative(entry.path(), path);
            fs::path value = path / key;
            redirectMap[key] = value.string();
            std::cout << "[BaseMod][ResourceRedirector][DEBUG] Registered: " << key.string() <<
                " --> " << value.string() << std::endl;
            count++;
        }
    }

    std::cout << "[BaseMod][ResourceRedirector][DEBUG] " << count <<
        " modded resources found in " << path.string() << std::endl;
}

void RegisterModResources(fs::path modDir) {
    try {
        if (std::filesystem::exists(modDir) && std::filesystem::is_directory(modDir)) {
            for (const auto& entry : std::filesystem::directory_iterator(modDir)) {
                RegisterFolderContents(entry / subFolder);
            }
        }
    } catch (std::exception e) {
        std::cout << "[BaseMod][ResourceRedirector] Error occured while registering modded resources" << std:: endl;
    }

    initialized = true;
}

const char* GetResourceOverride(const char* path) {
    if (!initialized) RegisterModResources(defaultModDir);

    return redirectMap.contains(path) ? redirectMap[path].c_str() : "";
}
