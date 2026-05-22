//
// Created by sunvy on 11/01/2026.
//

#ifndef SUNSETCRAFT_HORIZONTALBOX_H
#define SUNSETCRAFT_HORIZONTALBOX_H

#include "Slate.h"

namespace SunsetEngine
{
    class HorizontalBox : public Slate
    {
    public:

        void Draw() const override;

        void Clear();

        void Reserve(size_t size);

        void SetPadding(const glm::ivec2& padding);

        void SetAnchor(const glm::vec2& val);

        std::shared_ptr<Slate>& operator[](const uint8_t index);

        void AddChild(const std::shared_ptr<Slate>& child);

        [[nodiscard]]
        std::vector<std::shared_ptr<Slate>>::iterator begin();

        [[nodiscard]]
        std::vector<std::shared_ptr<Slate>>::iterator end();

    private:

        void Rebuild();

    protected:
        glm::ivec2 m_Padding = {0, 0};
        std::vector<std::shared_ptr<Slate>> m_Children;
    };
}

#endif //SUNSETCRAFT_HORIZONTALBOX_H