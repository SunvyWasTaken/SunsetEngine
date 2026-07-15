//
// Created by sunvy on 14/06/2026.
//

#pragma once

#define SUNSET_API __declspec(dllexport)

namespace Sunset
{
    template <typename T>
    inline constexpr bool always_false_v = false;
}
