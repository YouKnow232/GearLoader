#pragma once

#include "GearLoader/gearLoader.hpp"
#include <string>

class GLLogger {
    public:
        GLLogger(GearLoaderContext* ctx, GearLoaderApi* api) : glApi(api, ctx) { }
        void Debug(std::string str) {
            glApi.Log(GearLoader::LogLevel::DEBUG, str);
        }
        void Error(std::string str) {
            glApi.Log(GearLoader::LogLevel::ERR, str);
        }
        void Info(std::string str) {
            glApi.Log(GearLoader::LogLevel::INFO, str);
        }
        void Verbose(std::string str) {
            glApi.Log(GearLoader::LogLevel::VERBOSE, str);
        }
        void Warn(std::string str) {
            glApi.Log(GearLoader::LogLevel::WARN, str);
        }

    private:
        GearLoader::Api glApi;
};