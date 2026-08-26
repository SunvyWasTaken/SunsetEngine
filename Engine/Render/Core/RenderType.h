//
// Created by sunvy on 16/07/2026.
//

#pragma once

namespace Sunset
{
    enum class BlendFactor
    {
        Zero,
        One,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor
    };

    enum class CullMode
    {
        None,
        Back,
        Front
    };

    enum class PrimitiveType
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };

    struct RenderState
    {
        bool depthTest = true;
        bool depthWrite = true;

        bool blending = false;
        BlendFactor src = BlendFactor::One;
        BlendFactor dest = BlendFactor::Zero;

        CullMode cullMode = CullMode::Back;

        bool wireframe = false;
    };

    constexpr RenderState OpaqueState{
        .depthTest = true,
        .depthWrite = true,
        .blending = false,
        .cullMode = CullMode::None
    };

    constexpr RenderState TransparentState{
        .depthTest = true,
        .depthWrite = false,
        .blending = true,
        .src = BlendFactor::SrcAlpha,
        .dest = BlendFactor::OneMinusSrcAlpha,
        .cullMode = CullMode::Back
    };
}
