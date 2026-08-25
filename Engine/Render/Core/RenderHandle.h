//
// Created by sunvy on 25/08/2026.
//

#pragma once

namespace Sunset
{
    struct RenderHandle
    {
        constexpr RenderHandle() = default;

        explicit constexpr RenderHandle(const std::uint32_t resourceId)
            : id(resourceId)
        {
        }

        std::uint32_t operator()() const
        {
            return id;
        }

        [[nodiscard]]
        explicit operator bool() const
        {
            return id != 0;
        }

        std::uint32_t id = 0;
    };

    struct BufferHandle : RenderHandle { using RenderHandle::RenderHandle; };

    struct VertexArrayHandle : RenderHandle { using RenderHandle::RenderHandle; };

    struct TextureHandle : RenderHandle { using RenderHandle::RenderHandle; };

    struct ShaderHandle : RenderHandle { using RenderHandle::RenderHandle; };

    struct FramebufferHandle : RenderHandle { using RenderHandle::RenderHandle; };
}
