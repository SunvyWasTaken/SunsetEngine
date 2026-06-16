//
// Created by sunvy on 31/05/2026.
//

#pragma once

namespace Sunset
{
    struct ProfileData
    {
        static void Free();
        static std::vector<std::string>& Get();
    };

    struct Profiling final
    {
        explicit Profiling(const std::string_view& _name);

        ~Profiling();

        std::chrono::high_resolution_clock::time_point m_StartTimePoint;
        const std::string name;
    };
}

#ifdef SS_PROFILING
    #define SS_CONCAT(x, y) x##y
    #define SS_PROFILE_SCOPE(name) ::Sunset::Profiling SS_CONCAT(ProfileScope_, __LINE__)(name)
#ifndef _MSC_VER
    #define SS_PROFILE_FUNCTION() SS_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define SS_PROFILE_FUNCTION() SS_PROFILE_SCOPE(__FUNCSIG__)
#endif
#else
    #define SS_PROFILE_SCOPE(name)
    #define SS_PROFILE_FUNCTION()
#endif
