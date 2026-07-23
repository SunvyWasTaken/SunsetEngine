//
// Created by Codex on 23/07/2026.
//

#include "RenderPass.h"

namespace Sunset
{
    RenderPass::RenderPass(const std::string_view name)
        : m_Name(name)
    {
    }

    std::string_view RenderPass::GetName() const
    {
        return m_Name;
    }
}
