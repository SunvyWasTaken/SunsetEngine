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

    std::uint32_t RenderCommand::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        return GetRenderAPI().CreateBuffer(type, data, size, usage);
    }

    void RenderCommand::DestroyBuffer(const std::uint32_t buffer)
    {
        if (buffer == 0 || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyBuffer(buffer);
    }

    void RenderCommand::BindBuffer(const BufferType type, const std::uint32_t buffer)
    {
        GetRenderAPI().BindBuffer(type, buffer);
    }

    void RenderCommand::UpdateBuffer(const BufferType type, const std::uint32_t buffer, const size_t offset, const size_t size, const void* data)
    {
        GetRenderAPI().UpdateBuffer(type, buffer, offset, size, data);
    }

    std::uint32_t RenderCommand::CreateVertexArray()
    {
        return GetRenderAPI().CreateVertexArray();
    }

    void RenderCommand::DestroyVertexArray(const std::uint32_t vertexArray)
    {
        if (vertexArray == 0 || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyVertexArray(vertexArray);
    }

    void RenderCommand::BindVertexArray(const std::uint32_t vertexArray)
    {
        GetRenderAPI().BindVertexArray(vertexArray);
    }

    void RenderCommand::ConfigureVertexArray(const std::uint32_t vertexArray, const std::uint32_t vertexBuffer, const BufferLayout& layout)
    {
        GetRenderAPI().ConfigureVertexArray(vertexArray, vertexBuffer, layout);
    }

    std::uint32_t RenderCommand::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        return GetRenderAPI().CreateTexture2D(specification, data);
    }

    void RenderCommand::DestroyTexture(const std::uint32_t texture)
    {
        if (texture == 0 || !HasRenderAPI())
            return;

        GetRenderAPI().DestroyTexture(texture);
    }

    void RenderCommand::BindTexture(const std::uint32_t texture, const std::uint32_t slot)
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

    void RenderCommand::UpdateTexture2D(const std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        GetRenderAPI().UpdateTexture2D(texture, coord, specification, data);
    }
}
