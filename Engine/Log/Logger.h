//
// Created by sunvy on 04/01/2026.
//

#pragma once

#include <spdlog/spdlog.h>

namespace Sunset
{
    class Application;

    struct Log
    {
        friend Application;
    private:
        static void Init();

        static void Shutdown();
    public:
        static std::shared_ptr<spdlog::logger> InitLog(const std::string_view& name);

        static std::shared_ptr<spdlog::logger> GetLogger(std::string name);
    };

    struct PrintScreen
    {
        static void Add(const std::string_view& string);
        static void Clear();
        static std::vector<std::string>& Get();
    };
}
// #ifndef SS_RELEASE
#define INITLOG(name) Sunset::Log::InitLog(name);
/*
 * level : trace, debug, info, warn, error, critical.
 */
#define LOG(name, level, txt, ...) if (std::shared_ptr<spdlog::logger> _l = ::Sunset::Log::GetLogger(name)) {_l->level(std::format(txt, ##__VA_ARGS__));};
#define PRINTSCREEN(txt, ...) ::Sunset::PrintScreen::Add(std::format(txt, ##__VA_ARGS__));
// #else
// #define INITLOG(name)
// #define LOG(name, level, txt, ...)
// #define PRINTSCREEN(txt, ...)
// #endif
