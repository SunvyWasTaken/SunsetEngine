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
        void Paint(FormeDatas& out) override;
        void SetImage(TextureID id);
        void SetUv(const glm::vec4& uv);
    private:
        TextureID m_TextureID = 0;
        glm::vec4 m_Uv{0.f, 1.f, 1.f, 0.f};
    };

    template<>
    class WidgetBuilder<SRmGUI::Image>
    : public WidgetBuilderBase<SRmGUI::Image, WidgetBuilder<SRmGUI::Image>>
    {
    public:
        WidgetBuilder& Image(TextureID id)
        {
            m_Widget->SetImage(id);
            return *this;
        }

        WidgetBuilder& Uv(const glm::vec4& uv)
        {
            m_Widget->SetUv(uv);
            return *this;
        }
    };
} // SRmGUI