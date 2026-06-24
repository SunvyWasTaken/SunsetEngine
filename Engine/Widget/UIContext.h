//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    class Widget;
    class Button;

    class UIContext
    {
    public:
        ~UIContext();
        void SetRoot(const std::shared_ptr<Widget>& rootWidget);
        [[nodiscard]]
        std::shared_ptr<Widget> GetRoot() const;
        void Update(float dt);
        void PrePass();
        void Arrange(float width, float height);
        void ProcessInput();
        void ProcessMouseMove(const glm::ivec2& mousePosition);
        bool ProcessMouseButton(const glm::ivec2& mousePosition, bool pressed);
        void Paint();
        UIRenderList& GetRenderList();
    private:
        void ClearButtonState();
        std::shared_ptr<Widget> m_Root;
        UIRenderList m_RenderList;
        Widget* m_HoveredWidget = nullptr;
        Button* m_PressedButton = nullptr;
    };
} // Sunset