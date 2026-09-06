#include <filesystem>

/**
 * @brief Scans the mod folder for resource overrides
 * 
 * This function will be called by `GetResourceOverride` if not previously called.
 */
void RegisterModResources(std::filesystem::path modDir);
/**
 * @brief Gets the path of the modded resource file if one exists, else returns an empty string.
 * 
 * @param path the requested resource path
 * @return const char* The path of the modded resource
 */
const char* GetResourceOverride(const char* path);
