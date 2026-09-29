//
// Created by sunvy on 31/05/2026.
//

#pragma once

namespace Sunset
{
    struct ProfilEntry
    {
        std::string name;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
    };

    struct ProfileData
    {
        static void Free();
        static std::chrono::high_resolution_clock::time_point start;
        static std::chrono::high_resolution_clock::time_point end;
        static std::vector<ProfilEntry>& Get();
    };

    struct Profiling final
    {
        explicit Profiling(const std::string_view& _name);

        ~Profiling();

        ProfilEntry entry;
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
