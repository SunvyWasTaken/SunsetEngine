//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <cstdint>

namespace Sunset
{
    struct RenderResourceHandle
    {
        std::uint32_t id = 0;

        [[nodiscard]] explicit operator bool() const
        {
            return id != 0;
        }
    };

    struct BufferHandle : RenderResourceHandle {};
    struct VertexArrayHandle : RenderResourceHandle {};
    struct TextureHandle : RenderResourceHandle {};
    struct ShaderHandle : RenderResourceHandle {};
    struct FrameBufferHandle : RenderResourceHandle {};
}
