//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    class Material;
    class Mesh;

    class Drawable final
    {
    public:
        Drawable();
        ~Drawable();

        operator bool() const;

        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
    };
} // Sunset