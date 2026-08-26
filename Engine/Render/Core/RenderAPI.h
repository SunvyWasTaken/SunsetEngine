//
// Created by sunvy on 24/08/2026.
//

#pragma once
#include "RenderHandle.h"

namespace Sunset
{
    struct ShaderHandle;

    struct RenderAPI
    {
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void Clear(const glm::vec4& color) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        virtual std::unique_ptr<class Shader> CreateShader(const std::string& vertShader, const std::string& fragShader) = 0;
    };
} // Sunset