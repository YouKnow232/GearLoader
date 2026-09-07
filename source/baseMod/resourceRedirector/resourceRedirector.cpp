#include "resourceRedirector.h"
#include "gearLoaderLogger.h"
#include <filesystem>
#include <unordered_map>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

const fs::path defaultModDir = ".\\mods\\";
const fs::path subFolder = "Resource\\";

static bool initialized = false;
static std::unordered_map<fs::path, std::string> redirectMap;


void RegisterFolderContents(const fs::path& path, GLLogger* log) {
    if (!fs::is_directory(path)) return;

    int count = 0;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            fs::path key = fs::relative(entry.path(), path);
            fs::path value = path / key;
            redirectMap[key] = value.string();
            std::ostringstream ss1;
            ss1 << "Modded Resource Registered: " << key.string() << " --> " << value.string();
            if (log) log->Verbose(ss1.str());
            count++;
        }
    }

    std::ostringstream ss2;
    ss2 << count << " modded resources found in " << path.string();
    if (log) log->Info(ss2.str());
}

void RegisterModResources(fs::path modDir, GLLogger* log) {
    try {
        if (std::filesystem::exists(modDir) && std::filesystem::is_directory(modDir)) {
            for (const auto& entry : std::filesystem::directory_iterator(modDir)) {
                RegisterFolderContents(entry / subFolder, log);
            }
        }
    } catch (std::exception e) {
        if (log) log->Error("[BaseMod][ResourceRedirector] Error occured while registering modded resources");
    }

    initialized = true;
}

const char* GetResourceOverride(const char* path, GLLogger* log) {
    if (!initialized) RegisterModResources(defaultModDir, log);

    return redirectMap.contains(path) ? redirectMap[path].c_str() : "";
}
