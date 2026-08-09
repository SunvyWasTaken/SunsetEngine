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
