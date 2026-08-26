//
// Created by sunvy on 25/08/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    struct OpenGLRenderAPI final : RenderAPI
    {
        OpenGLRenderAPI();
        ~OpenGLRenderAPI() override;

        void Init() override;
        void Clear(const glm::vec4& color) override;
        void BeginFrame() override;
        void EndFrame() override;

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        std::unique_ptr<Shader> CreateShader(const std::string& vertShader, const std::string& fragShader) override;

        /*--------------------------------------*/
        /* Texture                              */
        /*--------------------------------------*/
        TextureHandle CreateTexture(const void* data = nullptr);

    private:

        struct OpenGLData;
        std::unique_ptr<OpenGLData> data;
    };
} // Sunset