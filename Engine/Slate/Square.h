//
// Created by sunvy on 11/01/2026.
//

#ifndef SUNSETCRAFT_SQUARE_H
#define SUNSETCRAFT_SQUARE_H

#include "Slate.h"

namespace SunsetEngine
{
    class Shader;

    class Square : public Slate
    {
    public:

        Square(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec4& color = glm::vec4{1.f}, int radius = 0);

        ~Square() override;

        void Draw() const override;

        void SetAnchor(const glm::vec2& val);

        void AddChild(const std::shared_ptr<Slate>& child);

        void SetPosition(const glm::ivec2& pos) override;

        void SetSize(const glm::ivec2& size) override;

        void SetRadius(int radius);

    private:

        void Clear();

        void Rebuild();

        void ComputePoints(std::vector<glm::vec2>& points);

    protected:
        glm::vec4 m_Color;
        int m_Radius;
        std::shared_ptr<Slate> m_Child;
    };
}

#endif //SUNSETCRAFT_SQUARE_H