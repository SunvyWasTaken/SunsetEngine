//
// Created by sunvy on 24/08/2026.
//

#pragma once

namespace Sunset
{
    class Camera;
    class Drawable;
    class VertexLayout;
    class Mesh;
    struct RenderState;
    class Pipeline;
    enum class BufferType;
    class Buffer;
    class Shader;
    class Texture;
    struct TextureDescription;
    struct ShaderHandle;

    struct RenderAPI
    {
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void Clear(const glm::vec4& color) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void Submit(const Drawable& drawable, const glm::mat4& transform) = 0;

        virtual void UseCamera(const Camera& camera) = 0;

        /****************************************/
        /* Shader                               */
        /****************************************/
        virtual std::unique_ptr<Shader> CreateShader(const std::string_view& vertShader, const std::string_view& fragShader) = 0;

        /****************************************/
        /* Buffer                               */
        /****************************************/
        virtual std::shared_ptr<Buffer> CreateBuffer(const BufferType& type) = 0;

        /****************************************/
        /* Texture                              */
        /****************************************/
        virtual std::unique_ptr<Texture> CreateTexture(const TextureDescription& desc) = 0;

        /****************************************/
        /* Pipeline                             */
        /****************************************/
        virtual std::shared_ptr<Pipeline> CreatePipeline(const RenderState& state) = 0;

        virtual std::shared_ptr<Mesh> CreateMesh(const std::shared_ptr<Buffer>& vertexBuffer, const std::shared_ptr<Buffer>& indexBuffer, const VertexLayout& vertexLayout) = 0;
    };
} // Sunset