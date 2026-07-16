//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Core/Event.h"
#include "Core/WindowSetting.h"

namespace Sunset
{
    class Window
    {
    public:
        [[nodiscard]]
        static std::unique_ptr<Window> CreateWindow(const WindowSetting& setting = {});

        virtual ~Window() = default;

        virtual void PollEvents() = 0;

        virtual void Present() = 0;

        [[nodiscard]]
        virtual bool ShouldClose() const = 0;

        virtual void RequestClose() = 0;

        virtual void SetVSync(bool enable) = 0;

        [[nodiscard]]
        virtual bool IsVSync() const = 0;

        [[nodiscard]]
        virtual glm::ivec2 Size() const = 0;

        virtual void BindEvent(const std::function<void(const Event::Type&)>& event) = 0;

        [[nodiscard]]
        virtual void* GetNativeHandle() const = 0;
    };
} // Sunset