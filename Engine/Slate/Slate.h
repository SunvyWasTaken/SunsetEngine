//
// Created by sunvy on 11/01/2026.
//

#ifndef SUNSETCRAFT_SLATE_H
#define SUNSETCRAFT_SLATE_H

namespace SunsetEngine
{
    class Drawable;

    class Slate
    {
    public:
        Slate();
        virtual ~Slate();

        virtual void Draw() const;

        [[nodiscard]]
        glm::ivec2 GetPosition() const;
        virtual void SetPosition(const glm::ivec2& position);

        [[nodiscard]]
        glm::ivec2 GetSize() const;
        virtual void SetSize(const glm::ivec2& size);

    protected:
        bool bIsDirty;
        glm::ivec2 m_Position;
        glm::ivec2 m_Size;
        glm::vec2 m_Anchor;

        std::unique_ptr<Drawable> m_Drawable;
    };
}


#endif //SUNSETCRAFT_SLATE_H