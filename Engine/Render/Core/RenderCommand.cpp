//
// Created by sunvy on 05/01/2026.
//

#include "RenderCommand.h"

#include "Render/Core/RenderAPI.h"
#include "Render/Resources/Texture.h"

namespace
{
    std::unique_ptr<Sunset::RenderAPI>& GetRenderAPIStorage()
    {
        static std::unique_ptr<Sunset::RenderAPI> api;
        return api;
    }

    Sunset::RenderAPI& GetRenderAPI()
    {
        auto& api = GetRenderAPIStorage();
        if (!api)
            throw std::runtime_error("Sunset Render API not available");

        return *api;
    }
}

namespace Sunset
{
    void RenderCommand::SetRenderAPI(std::unique_ptr<RenderAPI> api)
    {
        if (!api)
            return;

        api->Init();
        GetRenderAPIStorage() = std::move(api);
    }

    bool RenderCommand::HasRenderAPI()
    {
        return GetRenderAPIStorage() != nullptr;
    }

    void RenderCommand::BeginFrame() { GetRenderAPI().BeginFrame(); }
    void RenderCommand::EndFrame() { GetRenderAPI().EndFrame(); }
    void RenderCommand::Shutdown(){  GetRenderAPIStorage().reset(); }
    void RenderCommand::BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) { GetRenderAPI().BeginTarget(target, clearColor); }
    void RenderCommand::EndTarget() { GetRenderAPI().EndTarget(); }
    void RenderCommand::Flush() { GetRenderAPI().Flush(); }
    void RenderCommand::Submit(const Drawable& mesh, const glm::mat4& model) { GetRenderAPI().Submit(mesh, model); }
    void RenderCommand::DrawMesh(const Mesh& mesh, const RenderState& state) { GetRenderAPI().DrawMesh(mesh, state); }
    void RenderCommand::UseCamera(const Camera& camera) { GetRenderAPI().UseCamera(camera); }
    void RenderCommand::ShowCursor(bool show) { GetRenderAPI().ShowCursor(show); }
    void RenderCommand::SetViewport(const glm::ivec2& viewport) { GetRenderAPI().SetViewport(viewport); }
    void RenderCommand::SetCullMode(const CullMode mode) { GetRenderAPI().SetCullMode(mode); }

    FrameBufferCreateResult RenderCommand::CreateFrameBuffer(const FrameBufferSpecification& specification)
    {
        return GetRenderAPI().CreateFrameBuffer(specification);
    }

    void RenderCommand::DestroyFrameBuffer(const FrameBufferHandle framebuffer, const std::vector<TextureHandle>& colorAttachments, const TextureHandle depthAttachment)
    {
        if (!framebuffer || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyFrameBuffer(framebuffer, colorAttachments, depthAttachment);
    }

    void RenderCommand::BindFrameBuffer(const FrameBufferHandle framebuffer, const glm::ivec2& size)
    {
        GetRenderAPI().BindFrameBuffer(framebuffer, size);
    }

    void RenderCommand::UnbindFrameBuffer()
    {
        GetRenderAPI().UnbindFrameBuffer();
    }

    void RenderCommand::ClearFrameBuffer(const FrameBuffer& target, const ClearFlags flags, const glm::vec4& color, const float depth, const int stencil)
    {
        GetRenderAPI().ClearFrameBuffer(target, flags, color, depth, stencil);
    }

    void RenderCommand::ClearFrameBufferColor(const FrameBuffer& target, const std::uint32_t attachmentIndex, const glm::vec4& color)
    {
        GetRenderAPI().ClearFrameBufferColor(target, attachmentIndex, color);
    }

    void RenderCommand::ClearFrameBufferDepth(const FrameBuffer& target, const float depth)
    {
        GetRenderAPI().ClearFrameBufferDepth(target, depth);
    }

    void RenderCommand::ClearFrameBufferStencil(const FrameBuffer& target, const int stencil)
    {
        GetRenderAPI().ClearFrameBufferStencil(target, stencil);
    }

    void RenderCommand::BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, const ClearFlags mask)
    {
        GetRenderAPI().BlitFrameBuffer(source, target, mask);
    }

    bool RenderCommand::IsFrameBufferValid(const FrameBufferHandle framebuffer)
    {
        return HasRenderAPI() && GetRenderAPI().IsFrameBufferValid(framebuffer);
    }

    BufferHandle RenderCommand::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        return GetRenderAPI().CreateBuffer(type, data, size, usage);
    }

    void RenderCommand::DestroyBuffer(const BufferHandle buffer)
    {
        if (!buffer || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyBuffer(buffer);
    }

    void RenderCommand::BindBuffer(const BufferType type, const BufferHandle buffer)
    {
        GetRenderAPI().BindBuffer(type, buffer);
    }

    void RenderCommand::UpdateBuffer(const BufferType type, const BufferHandle buffer, const size_t offset, const size_t size, const void* data)
    {
        GetRenderAPI().UpdateBuffer(type, buffer, offset, size, data);
    }

    VertexArrayHandle RenderCommand::CreateVertexArray()
    {
        return GetRenderAPI().CreateVertexArray();
    }

    void RenderCommand::DestroyVertexArray(const VertexArrayHandle vertexArray)
    {
        if (!vertexArray || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyVertexArray(vertexArray);
    }

    void RenderCommand::BindVertexArray(const VertexArrayHandle vertexArray)
    {
        GetRenderAPI().BindVertexArray(vertexArray);
    }

    void RenderCommand::ConfigureVertexArray(const VertexArrayHandle vertexArray, const BufferHandle vertexBuffer, const BufferLayout& layout)
    {
        GetRenderAPI().ConfigureVertexArray(vertexArray, vertexBuffer, layout);
    }

    TextureHandle RenderCommand::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        return GetRenderAPI().CreateTexture2D(specification, data);
    }

    void RenderCommand::DestroyTexture(const TextureHandle texture)
    {
        if (!texture || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyTexture(texture);
    }

    void RenderCommand::BindTexture(const TextureHandle texture, const std::uint32_t slot)
    {
        GetRenderAPI().BindTexture(texture, slot);
    }

    void RenderCommand::BindTexture(const Texture& texture, const std::uint32_t slot)
    {
        BindTexture(texture.GetRendererId(), slot);
    }

    void RenderCommand::BindTexture(const Textures& texture, const std::uint32_t slot)
    {
        BindTexture(texture.GetRendererId(), slot);
    }

    void RenderCommand::UpdateTexture2D(const TextureHandle texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        GetRenderAPI().UpdateTexture2D(texture, coord, specification, data);
    }

    ShaderHandle RenderCommand::CreateShader(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        return GetRenderAPI().CreateShader(vertexSource, fragmentSource);
    }

    void RenderCommand::DestroyShader(const ShaderHandle shader)
    {
        if (!shader || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyShader(shader);
    }

    void RenderCommand::BindShader(const ShaderHandle shader)
    {
        GetRenderAPI().BindShader(shader);
    }

    void RenderCommand::SetShaderFloat(const ShaderHandle shader, const std::string_view name, const float value)
    {
        GetRenderAPI().SetShaderFloat(shader, name, value);
    }

    void RenderCommand::SetShaderInt(const ShaderHandle shader, const std::string_view name, const int value)
    {
        GetRenderAPI().SetShaderInt(shader, name, value);
    }

    void RenderCommand::SetShaderVec2(const ShaderHandle shader, const std::string_view name, const glm::vec2& value)
    {
        GetRenderAPI().SetShaderVec2(shader, name, value);
    }

    void RenderCommand::SetShaderVec3(const ShaderHandle shader, const std::string_view name, const glm::vec3& value)
    {
        GetRenderAPI().SetShaderVec3(shader, name, value);
    }

    void RenderCommand::SetShaderVec4(const ShaderHandle shader, const std::string_view name, const glm::vec4& value)
    {
        GetRenderAPI().SetShaderVec4(shader, name, value);
    }

    void RenderCommand::SetShaderMat4(const ShaderHandle shader, const std::string_view name, const glm::mat4& value)
    {
        GetRenderAPI().SetShaderMat4(shader, name, value);
    }
}
