//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "Render/Drawable.h"

namespace Sunset
{
    struct Component
    {
        Component() = default;
        virtual ~Component() = default;
    };

    struct TagComponent : public Component
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

    };

    struct MeshComponent : public Component
    {
        Drawable m_mesh;
    };
} // Sunset