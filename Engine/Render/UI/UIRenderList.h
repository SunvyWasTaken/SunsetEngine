//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include <glm/glm.hpp>

namespace Sunset
{
    namespace UIDraw
    {
        struct Base
        {
            glm::vec2 Position {0, 0};
            glm::vec2 Size {0, 0};
            glm::vec4 Color {1.0f};
        };

        struct Rectangle : Base
        {
            int Radius = 0;
        };
        struct Image : Base
        {
            uint32_t TextureId = 0;
            glm::vec4 Uv{0.f, 0.f, 1.f, 1.f};
        };
        struct Text : Base
        {
            std::string text;
        };

        using Type = std::variant<Rectangle, Image, Text>;
    }

    class UIRenderList
    {
    public:
        void Clear();

        void AddRectangle(const glm::ivec2& position, const glm::ivec2& size, const glm::vec4& color, int radius = 0);

        void AddImage(const glm::ivec2& position, const glm::ivec2& size, const glm::vec4& color, uint32_t textureId, const glm::vec4& uv);

        void AddText(const std::string& text, const glm::ivec2& position, const glm::ivec2& size = {15, 15}, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

        [[nodiscard]] bool IsEmpty() const;

        std::vector<UIDraw::Type> m_UIDrawList;
    };
} // Sunset
