//
// Created by sunvy on 29/08/2026.
//

#pragma once

#include <glm/fwd.hpp>

namespace Sunset
{
    class Drawable;

    struct DrawCmd final
    {
        DrawCmd(const Drawable* drawable, const glm::mat4& modelMatrix)
            : m_Drawable(drawable), transform(modelMatrix)
        {}

        const Drawable* m_Drawable;
        glm::mat4 transform;
    };
}
