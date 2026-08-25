//
// Created by sunvy on 25/08/2026.
//

#include "RenderCommand.h"

#include "RenderAPI.h"

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
            throw std::runtime_error("Sunset::RenderAPI is nullptr");
        return *api;
    }
}

namespace Sunset
{
    void RenderCommand::SetRenderAPI(std::unique_ptr<RenderAPI> renderAPI)
    {
        if (!renderAPI)
        {
            LOG("Engine", error, "The given RenderAPI is null")
            return;
        }

        renderAPI->Init();
        GetRenderAPIStorage() = std::move(renderAPI);
    }

    bool RenderCommand::HasRenderAPI()
    {
        return GetRenderAPIStorage() != nullptr;
    }

    void RenderCommand::Shutdown()
    {
        GetRenderAPIStorage().reset();
    }

    void RenderCommand::BeginFrame()
    {
        GetRenderAPI().BeginFrame();
    }

    void RenderCommand::EndFrame()
    {
        GetRenderAPI().EndFrame();
    }
} // Sunset