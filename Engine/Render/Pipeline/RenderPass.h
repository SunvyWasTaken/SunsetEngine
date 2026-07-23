//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <string_view>

namespace Sunset
{
    class RenderPass
    {
    public:
        explicit RenderPass(std::string_view name);
        virtual ~RenderPass() = default;

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;

        [[nodiscard]] std::string_view GetName() const;

    private:
        std::string_view m_Name;
    };
}
