//
// Created by sunvy on 25/08/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    struct OpenGLRenderAPI : public RenderAPI
    {
        ~OpenGLRenderAPI();

        void Init() override;
        void Clear(const glm::vec4& color) override;
        void BeginFrame() override;
        void EndFrame() override;

    private:

        struct OpenGLData;
        std::unique_ptr<OpenGLData> data;
    };
} // Sunset