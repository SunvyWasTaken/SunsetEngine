//
// Created by sunvy on 05/01/2026.
//

#include "RenderCommand.h"

#include "Backend/OpenGL/OpenGLGraphicsDevice.h"
#include "Render/Core/RenderAPI.h"

namespace
{
    std::unique_ptr<Sunset::RenderAPI> CreateRenderAPI()
    {
        return std::make_unique<Sunset::OpenGLGraphicsDevice>();
    }

    std::unique_ptr<Sunset::RenderAPI>& GetRenderAPIStorage()
    {
        static std::unique_ptr<Sunset::RenderAPI> api = CreateRenderAPI();
        return api;
    }

    Sunset::RenderAPI& GetRenderAPI()
    {
        auto& api = GetRenderAPIStorage();
        if (!api)
            api = CreateRenderAPI();

        return *api;
    }
}

namespace Sunset
{
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