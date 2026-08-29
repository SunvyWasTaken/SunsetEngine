//
// Created by sunvy on 09/08/2026.
//

#pragma once

#include "Component.h"

#include "Render/Resources/Drawable.h"

namespace Sunset
{
    class SpriteRenderComponent : public Component
    {
        friend class BuildRenderScene;
    public:
        SpriteRenderComponent();
    private:
        Drawable m_drawable;
    };

    template <typename Archive>
    void Serialize(Archive& ar, SpriteRenderComponent& component);
} // Sunset