//
// Created by sunvy on 05/01/2026.
//

#include "RenderCommand.h"

#include "Render/Core/RenderAPI.h"

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

    void RenderCommand::BeginFrame() { GetRenderAPI().BeginFrame(); }
    void RenderCommand::EndFrame() { GetRenderAPI().EndFrame(); }
    void RenderCommand::Shutdown(){  GetRenderAPIStorage().reset(); }
    void RenderCommand::BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) { GetRenderAPI().BeginTarget(target, clearColor); }
    void RenderCommand::EndTarget() { GetRenderAPI().EndTarget(); }
    void RenderCommand::Flush() { GetRenderAPI().Flush(); }
    void RenderCommand::Submit(const Drawable& mesh, const glm::mat4& model) { GetRenderAPI().Submit(mesh, model); }
    void RenderCommand::UseCamera(const Camera& camera) { GetRenderAPI().UseCamera(camera); }
    void RenderCommand::ShowCursor(bool show) { GetRenderAPI().ShowCursor(show); }
    void RenderCommand::SetViewport(const glm::ivec2& viewport) { GetRenderAPI().SetViewport(viewport); }
}