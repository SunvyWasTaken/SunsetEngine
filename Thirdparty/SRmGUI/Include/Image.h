//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Type.h"
#include "Widget.h"

namespace SRmGUI
{
    class Image : public Widget
    {
    public:
        void Update(float dt) override;
        void Arrange(const Rect& viewportRect) override;
        void Paint(FormeDatas& out) override;
        void SetImage(TextureID id);
        void SetUv(const glm::vec4& uv);
    private:
        TextureID m_TextureID = 0;
        glm::vec4 m_Uv;
    };
} // SRmGUI