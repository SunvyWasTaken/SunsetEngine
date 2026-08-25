//
// Created by sunvy on 25/08/2026.
//

#include "RenderCommand.h"

#include "RenderAPI.h"
#include "RenderHandle.h"

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

/*--------------------------------------*/
/* Shader                               */
/*--------------------------------------*/
namespace Sunset
{
    ShaderHandle RenderCommand::CreateShader(const std::string &vertShader, const std::string &fragShader)
    {
        return GetRenderAPI().CreateShader(vertShader, fragShader);
    }

    void RenderCommand::DestroyShader(ShaderHandle &shaderID)
    {
        GetRenderAPI().DestroyShader(shaderID);
        shaderID = ShaderHandle();
    }

    void RenderCommand::BindShader(const ShaderHandle &shaderID)
    {
        GetRenderAPI().BindShader(shaderID);
    }

    void RenderCommand::SetShaderFloat(const ShaderHandle &shaderID, const std::string_view &name, float value)
    {
        GetRenderAPI().SetShaderFloat(shaderID, name, value);
    }

    void RenderCommand::SetShaderInt(const ShaderHandle &shaderID, const std::string_view &name, int value)
    {
        GetRenderAPI().SetShaderInt(shaderID, name, value);
    }

    void RenderCommand::SetShaderVec2(const ShaderHandle &shaderID, const std::string_view &name,
        const glm::vec2 &value)
    {
        GetRenderAPI().SetShaderVec2(shaderID, name, value);
    }

    void RenderCommand::SetShaderVec3(const ShaderHandle &shaderID, const std::string_view &name,
        const glm::vec3 &value)
    {
        GetRenderAPI().SetShaderVec3(shaderID, name, value);
    }

    void RenderCommand::SetShaderVec4(const ShaderHandle &shaderID, const std::string_view &name,
        const glm::vec4 &value)
    {
        GetRenderAPI().SetShaderVec4(shaderID, name, value);
    }

    void RenderCommand::SetShaderMat4(const ShaderHandle &shaderID, const std::string_view &name,
        const glm::mat4 &value)
    {
        GetRenderAPI().SetShaderMat4(shaderID, name, value);
    }
} // Sunset

/*--------------------------------------*/
/* Texture                              */
/*--------------------------------------*/
namespace Sunset
{
    TextureHandle RenderCommand::CreateTexture(const void *data)
    {
        return GetRenderAPI().CreateTexture(data);
    }

    void RenderCommand::DestroyTexture(TextureHandle &textureID)
    {
        GetRenderAPI().DestroyTexture(textureID);
        textureID = TextureHandle();
    }

    void RenderCommand::BindTexture(const TextureHandle &texture, std::uint32_t slot)
    {
        GetRenderAPI().BindTexture(texture, slot);
    }
} // Sunset