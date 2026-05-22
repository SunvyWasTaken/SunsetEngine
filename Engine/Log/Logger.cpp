//
// Created by sunvy on 04/01/2026.
//

#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    std::unordered_map<std::string_view, std::shared_ptr<spdlog::logger>> m_Loggers;
    spdlog::sink_ptr m_Sink;

    std::vector<std::string> m_PrintScreen;
}

namespace SunsetEngine
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_Sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    }

    void Log::Shutdown()
    {
        m_Sink.reset();
        m_Loggers.clear();
    }

    std::shared_ptr<spdlog::logger> Log::InitLog(const std::string_view& name)
    {
        if (m_Loggers.contains(name))
            return m_Loggers[name];

        auto logger = std::make_shared<spdlog::logger>(name.data(), m_Sink);
#ifdef NDEBUG
        logger->set_level(spdlog::level::info);
#else
        logger->set_level(spdlog::level::trace);
#endif
        spdlog::register_logger(logger);
        m_Loggers[name] = logger;
        return logger;
    }

    std::shared_ptr<spdlog::logger> Log::GetLogger(std::string name)
    {
        const auto it = m_Loggers.find(name);
        return it == m_Loggers.end() ? nullptr : it->second;
    }

    void PrintScreen::Add(const std::string_view& string)
    {
        m_PrintScreen.emplace_back(string);
    }

    void PrintScreen::Clear()
    {
        m_PrintScreen.clear();
    }

    std::vector<std::string>& PrintScreen::Get()
    {
        return m_PrintScreen;
    }
}
