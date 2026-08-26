//
// Created by sunvy on 15/01/2026.
//

#ifndef SUNSETCRAFT_DEBUGGLM_H
#define SUNSETCRAFT_DEBUGGLM_H

#include <glm/glm.hpp>

template <typename T>
struct std::formatter<glm::vec<3, T, glm::defaultp>> : std::formatter<T>
{
    template <typename FormatContext>
    auto format(const glm::vec<3, T, glm::defaultp>& v, FormatContext& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "(x:{}, y:{}, z:{})",
            v.x, v.y, v.z
        );
    }
};

template <typename T>
struct std::formatter<glm::vec<2, T, glm::defaultp>> : std::formatter<T>
{
    template <typename FormatContext>
auto format(const glm::vec<2, T, glm::defaultp>& v, FormatContext& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "(x:{}, y:{})",
            v.x, v.y
        );
    }
};

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

#if SS_DEBUG
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#include <csignal>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif
#else
#define DEBUG_BREAK()
#endif

#endif //SUNSETCRAFT_DEBUGGLM_H