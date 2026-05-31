//
// Created by sunvy on 31/05/2026.
//

#include "Profiling.h"

namespace
{
    std::vector<std::string> ProfilingData;

    void Add(const std::string& data)
    {
        ProfilingData.emplace_back(data);
    }

    #define SS_ADD_PROFILING_DATA(data, ...) ::Add(std::format(data, ##__VA_ARGS__));
}

namespace Sunset
{
    void ProfileData::Free()
    {
        ProfilingData.clear();
    }

    std::vector<std::string> & ProfileData::Get()
    {
        return ProfilingData;
    }

    Profiling::Profiling(const std::string_view &_name)
        : name(_name)
    {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }

    Profiling::~Profiling()
    {
        const auto endTimePoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        const float duration = (end - start) * 0.001f;
        SS_ADD_PROFILING_DATA("{:.3f}ms : {}", duration, name);
    }
}
