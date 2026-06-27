//
// Created by sunvy on 27/06/2026.
//

#pragma once

#include "LeafWidget.h"

namespace Sunset
{
    class UIText : public LeafWidget
    {
    public:
        UIText() = default;
        explicit UIText(std::string text);

        void ComputeDesiredSize() override;
        void Paint(UIRenderList& renderList) override;

        void SetText(std::string text);
        [[nodiscard]] const std::string& GetText() const;

        void SetColor(const glm::vec4& color);
        [[nodiscard]] const glm::vec4& GetColor() const;

        void SetGlyphSize(const glm::ivec2& glyphSize);
        [[nodiscard]] const glm::ivec2& GetGlyphSize() const;

    private:
        std::string m_Text;
        glm::vec4 m_Color {1.f, 1.f, 1.f, 1.f};
        glm::ivec2 m_GlyphSize {15, 15};
    };
} // Sunset