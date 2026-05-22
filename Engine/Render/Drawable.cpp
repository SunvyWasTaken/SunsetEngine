//
// Created by sunvy on 15/12/2025.
//

#include "Drawable.h"

#include "Material.h"

namespace SunsetEngine
{
    Drawable::Drawable()
        : m_Mesh(nullptr)
        , m_Material(std::make_shared<Material>())
        , m_Position(0.f)
        , m_RenderState({})
    {
    }

    Drawable::~Drawable()
    {
    }
}
