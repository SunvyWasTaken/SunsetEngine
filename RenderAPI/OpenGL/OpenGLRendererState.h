//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderType.h"

namespace Sunset::OpenGLRendererState
{
    void Reset();
    void Apply(const RenderState& state);
    void SetCullMode(CullMode mode);
}
