//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Rect.hpp"
#include "SRmGUI_fwd.h"
#include "Type.h"

namespace SRmGUI
{
    class Context
    {
    public:
        Context();
        ~Context();
        void SetRoot(const WidgetPtr& root);

        void Update(float dt);
        void HandleEvent();
        void HandleMousePos(const glm::vec2& mousePos);
        bool HandleMouseEvent(uint8_t type, uint8_t key);
        void Arrange(const Rect& viewportRect);
        void Paint();
        const FormeDatas& GetData();

        [[nodiscard]]
        glm::vec2 GetWindowSize() const;

    private:
        WidgetPtr m_Root = nullptr;
        Rect m_ViewportRect;
        FormeDatas m_FormeDatas;
    };
} // SRmGUi