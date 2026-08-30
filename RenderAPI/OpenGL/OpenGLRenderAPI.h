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

        void BeginScene(const Camera &camera) override;

        void EndScene() override;

        void Submit(const Drawable &drawable, const glm::mat4 &transform) override;

        void UseCamera(const Camera &camera) override;

        void SetViewport(const glm::ivec2 &screenSize) override;

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        std::unique_ptr<Shader> CreateShader(const std::string_view& vertShader, const std::string_view& fragShader) override;

        /****************************************/
        /* Buffer                               */
        /****************************************/
        std::shared_ptr<Buffer> CreateBuffer(const BufferType& type) override;


        /*--------------------------------------*/
        /* Texture                              */
        /*--------------------------------------*/
        std::unique_ptr<Texture> CreateTexture(const TextureDescription &desc) override;

        /****************************************/
        /* Pipeline                             */
        /****************************************/
        std::shared_ptr<Pipeline> CreatePipeline(const RenderState& state) override;

        std::shared_ptr<Mesh> CreateMesh(const std::shared_ptr<Buffer> &vertexBuffer,
            const std::shared_ptr<Buffer> &indexBuffer, const VertexLayout &vertexLayout) override;

        std::shared_ptr<RenderTarget> CreateFramebuffer(const RenderTargetSpecification &spec) override;

    private:

        struct OpenGLData;
        std::unique_ptr<OpenGLData> data;
    };
} // Sunset