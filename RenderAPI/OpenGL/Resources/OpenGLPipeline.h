//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Core/RenderType.h"
#include "Render/Resources/Pipeline.h"

namespace Sunset
{
    class OpenGLPipeline final : public Pipeline
    {
    public:
        explicit OpenGLPipeline(const RenderState& state);

        ~OpenGLPipeline() override;

        void Bind() override;

    private:
        RenderState m_RenderState;
    };
} // Sunset