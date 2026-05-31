//
// Created by sunvy on 31/05/2026.
//

#pragma once

namespace Sunset
{
    struct Profiling
    {
        std::chrono::steady_clock::time_point start;
        const std::string name;
        explicit Profiling(const std::string_view& _name)
            : name(_name)
        {
            start = std::chrono::steady_clock::now();
        }

        ~Profiling()
        {
            const auto end = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            PRINTSCREEN("{} : {}ms", name, duration.count());
        }
    };
}

#ifdef SS_PROFILING
    #define SS_PROFILE_SCOPE(name) ::Sunset::Profiling name##__LINE__(#name)
    #define SS_PROFILE_FUNCTION() SS_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define SS_PROFILE_SCOPE(name)
    #define SS_PROFILE_FUNCTION()
#endif
