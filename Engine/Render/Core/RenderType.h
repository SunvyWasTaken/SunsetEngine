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

    enum class RenderLayer
    {
        World,
        Overlay
    };

    struct RenderState
    {
        RenderLayer layer = RenderLayer::World;

        // Use this for first-person hands/items, HUD meshes, or any geometry that
        // must be rendered after the world without being hidden by the world depth buffer.
        static RenderState AfterWorldOverlay();

        bool depthTest = true;
        bool depthWrite = true;

        bool blending = false;
        BlendFactor src = BlendFactor::One;
        BlendFactor dest = BlendFactor::Zero;

        CullMode cullMode = CullMode::Front;
        bool wireframe = false;
        bool DrawInstance = false;
        std::uint32_t nbrInstance = 0;
        bool HasIndice = true;
        PrimitiveType primitiveType = PrimitiveType::Triangles;
    };
}
