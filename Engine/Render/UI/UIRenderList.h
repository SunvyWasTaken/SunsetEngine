//
// Created by sunvy on 24/06/2026.
//

#pragma once

namespace Sunset
{
    class UIRenderList
    {
    public:
        void Clear();

        void AddRectangle(const glm::ivec2& position, const glm::ivec2& size, const glm::vec4& color, int radius = 0);
        [[nodiscard]] bool IsEmpty() const;

    private:

        struct RectangleCommand
        {
            glm::ivec2 Position {0, 0};
            glm::ivec2 Size {0, 0};
            glm::vec4 Color {1.0f};
            int Radius = 0;
        };

        std::vector<RectangleCommand> m_Rectangles;
    };
} // Sunset