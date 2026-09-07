#pragma once

#include "gearLoaderLogger.h"
#include <filesystem>

/**
 * @brief Scans the mod folder for resource overrides
 * 
 * This function will be called by `GetResourceOverride` if not previously called.
 */
void RegisterModResources(std::filesystem::path modDir, GLLogger* log);
/**
 * @brief Gets the path of the modded resource file if one exists, else returns an empty string.
 * 
 * @param path The requested resource given as a relative path from the Resource folder.
 * @return const char* The path of the modded resource
 */
const char* GetResourceOverride(const char* path, GLLogger* log);
