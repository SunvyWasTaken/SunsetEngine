//
// Created by sunvy on 15/12/2025.
//

#include "Drawable.h"

#include "Material.h"

namespace Sunset
{
    Drawable::Drawable()
        : m_Mesh(nullptr)
        , m_Material(std::make_shared<Material>())
        , m_Position(0.f, 0.f, 0.f)
        , m_Scale(1.f, 1.f, 1.f)
        , m_RenderState({})
    {
    }

    Drawable::~Drawable()
    {
    }

    Drawable::operator bool() const
    {
        return m_Mesh != nullptr && m_Material != nullptr;
    }
}
