//
// Created by sunvy on 31/05/2026.
//

#include "Profiling.h"

namespace
{
    std::mutex ProfilingDataMutex;

    std::vector<Sunset::ProfilEntry> ProfilingData;

    void Add(Sunset::ProfilEntry& data)
    {
        std::scoped_lock lock(ProfilingDataMutex);
        ProfilingData.emplace_back(data);
    }
}

namespace Sunset
{
    void ProfileData::Free()
    {
        std::scoped_lock lock(ProfilingDataMutex);
        ProfilingData.clear();
    }

    std::vector<ProfilEntry> & ProfileData::Get()
    {
        std::scoped_lock lock(ProfilingDataMutex);
        return ProfilingData;
    }

    Profiling::Profiling(const std::string_view &_name)
    {
        entry.name = _name;
        entry.start = std::chrono::high_resolution_clock::now();
    }

    Profiling::~Profiling()
    {
        entry.end = std::chrono::high_resolution_clock::now();
        ::Add(entry);
    }
}
