//
// Created by sunvy on 24/06/2026.
//

#pragma once
#include <memory>

#include "Widget.h"
#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    class UIContext
    {
    public:
        void SetRoot(const std::shared_ptr<Widget>& rootWidget);
        void Update(float dt);
        void PrePass();
        void Arrange(float width, float height);
        void ProcessInput();
        void Paint();
        UIRenderList& GetRenderList();
    private:
        std::shared_ptr<Widget> m_Root;
        UIRenderList m_RenderList;
    };
} // Sunset