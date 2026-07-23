//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <cstdint>

namespace Sunset
{
    struct RenderResourceHandle
    {
        constexpr RenderResourceHandle() = default;

        explicit constexpr RenderResourceHandle(const std::uint32_t resourceId)
            : id(resourceId)
        {
        }

        std::uint32_t id = 0;

        [[nodiscard]] explicit operator bool() const
        {
            return id != 0;
        }
    };

    struct BufferHandle : RenderResourceHandle { using RenderResourceHandle::RenderResourceHandle; };
    struct VertexArrayHandle : RenderResourceHandle { using RenderResourceHandle::RenderResourceHandle; };
    struct TextureHandle : RenderResourceHandle { using RenderResourceHandle::RenderResourceHandle; };
    struct ShaderHandle : RenderResourceHandle { using RenderResourceHandle::RenderResourceHandle; };
    struct FrameBufferHandle : RenderResourceHandle { using RenderResourceHandle::RenderResourceHandle; };
}
