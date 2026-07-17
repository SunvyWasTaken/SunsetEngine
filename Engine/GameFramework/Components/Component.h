//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "../../Render/Resources/Drawable.h"
#include "Reflection/ReflectionType.h"

namespace Sunset
{
    struct Component
    {
        Component() = default;
        virtual ~Component();

        virtual ReflectionType Properties();
    };

    struct TagComponent : public Component
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

        ReflectionType Properties() override;
    };

    struct MeshComponent : public Component
    {
        Drawable m_mesh;
    };
} // Sunset