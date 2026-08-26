//
// Created by sunvy on 15/12/2025.
//

#include "Drawable.h"

namespace Sunset
{
    Drawable::Drawable()
    {
    }

    Drawable::~Drawable()
    {
    }

    Drawable::operator bool() const
    {
        return m_Mesh != nullptr && m_Material != nullptr;
    }
} // Sunset