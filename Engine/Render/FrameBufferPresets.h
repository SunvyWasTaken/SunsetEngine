//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "FrameBufferSpecification.h"

namespace Sunset::FrameBufferPresets
{
    [[nodiscard]] FrameBufferSpecification Viewport(int width, int height, int samples = 1);
    [[nodiscard]] FrameBufferSpecification Scene(int width, int height, int samples = 1);
    [[nodiscard]] FrameBufferSpecification HDRScene(int width, int height, int samples = 1);
    [[nodiscard]] FrameBufferSpecification ShadowMap(int size);
    [[nodiscard]] FrameBufferSpecification PostProcess(int width, int height);
    [[nodiscard]] FrameBufferSpecification Blur(int width, int height);
}
