//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <cstdint>
#include <vector>

namespace Sunset
{
    enum class ClearFlags : std::uint8_t
    {
        None = 0,
        Color = 1 << 0,
        Depth = 1 << 1,
        Stencil = 1 << 2
    };

    constexpr ClearFlags operator|(const ClearFlags left, const ClearFlags right)
    {
        return static_cast<ClearFlags>(static_cast<std::uint8_t>(left) | static_cast<std::uint8_t>(right));
    }

    constexpr ClearFlags operator&(const ClearFlags left, const ClearFlags right)
    {
        return static_cast<ClearFlags>(static_cast<std::uint8_t>(left) & static_cast<std::uint8_t>(right));
    }

    constexpr ClearFlags& operator|=(ClearFlags& left, const ClearFlags right)
    {
        left = left | right;
        return left;
    }

    constexpr bool HasClearFlag(const ClearFlags flags, const ClearFlags flag)
    {
        return (flags & flag) != ClearFlags::None;
    }

    enum class FrameBufferTextureFormat
    {
        None = 0,
        RGBA8,
        RGBA16F,
        R32F,
        Depth24Stencil8,
        Depth32F
    };

    enum class TextureFilter
    {
        Nearest,
        Linear
    };

    enum class TextureWrap
    {
        ClampToEdge,
        ClampToBorder,
        Repeat
    };

    struct FrameBufferTextureSpecification
    {
        FrameBufferTextureFormat format = FrameBufferTextureFormat::RGBA8;
        TextureFilter filter = TextureFilter::Nearest;
        TextureWrap wrap = TextureWrap::ClampToEdge;
    };

    struct FrameBufferSpecification
    {
        int width = 1;
        int height = 1;
        int samples = 1;
        std::vector<FrameBufferTextureSpecification> attachments = {
            { FrameBufferTextureFormat::RGBA8 },
            { FrameBufferTextureFormat::Depth24Stencil8 }
        };
    };
}
