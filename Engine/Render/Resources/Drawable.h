//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Render/Core/RenderType.h"

namespace Sunset
{
    class Material;
    class Textures;
    class Shader;
    class Mesh;

    class Drawable final
    {
    public:
        Drawable();
        ~Drawable();

        operator bool() const;

        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
        RenderState m_RenderState;
    };
}
