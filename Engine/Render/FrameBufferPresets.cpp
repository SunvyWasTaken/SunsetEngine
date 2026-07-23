//
// Created by Codex on 23/07/2026.
//

#include "FrameBufferPresets.h"

namespace Sunset::FrameBufferPresets
{
    FrameBufferSpecification Viewport(const int width, const int height, const int samples)
    {
        return {
            .width = width,
            .height = height,
            .samples = samples,
            .attachments = {
                { FrameBufferTextureFormat::RGBA8, TextureFilter::Linear, TextureWrap::ClampToEdge },
                { FrameBufferTextureFormat::Depth24Stencil8 }
            }
        };
    }

    FrameBufferSpecification Scene(const int width, const int height, const int samples)
    {
        return Viewport(width, height, samples);
    }

    FrameBufferSpecification HDRScene(const int width, const int height, const int samples)
    {
        return {
            .width = width,
            .height = height,
            .samples = samples,
            .attachments = {
                { FrameBufferTextureFormat::RGBA16F, TextureFilter::Linear, TextureWrap::ClampToEdge },
                { FrameBufferTextureFormat::Depth24Stencil8 }
            }
        };
    }

    FrameBufferSpecification ShadowMap(const int size)
    {
        return {
            .width = size,
            .height = size,
            .samples = 1,
            .attachments = {
                { FrameBufferTextureFormat::Depth32F, TextureFilter::Linear, TextureWrap::ClampToBorder }
            }
        };
    }

    FrameBufferSpecification PostProcess(const int width, const int height)
    {
        return {
            .width = width,
            .height = height,
            .samples = 1,
            .attachments = {
                { FrameBufferTextureFormat::RGBA16F, TextureFilter::Linear, TextureWrap::ClampToEdge }
            }
        };
    }

    FrameBufferSpecification Blur(const int width, const int height)
    {
        return PostProcess(width, height);
    }
}
