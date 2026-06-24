//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Core/Input.h"
#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    class Widget;
    class Button;

    class UIContext
    {
    public:
        ~UIContext();
        void AddWidget(const std::shared_ptr<Widget>& rootWidget);
        void Update(float dt);
        void PrePass();
        void Arrange(float width, float height);
        void ProcessInput();
        void ProcessMouseMove(const glm::ivec2& mousePosition);
        bool ProcessMouseButton(unsigned int button, Event::Action action, const glm::ivec2& mousePosition);
        void Paint();
        UIRenderList& GetRenderList();
    private:
        std::vector<std::shared_ptr<Widget>> m_Roots;
        UIRenderList m_RenderList;
        Widget* m_HoveredWidget = nullptr;
        Button* m_PressedButton = nullptr;
    };
} // Sunset
