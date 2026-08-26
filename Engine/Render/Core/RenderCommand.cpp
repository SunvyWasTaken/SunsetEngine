//
// Created by sunvy on 25/08/2026.
//

#include "RenderCommand.h"
#include "RenderAPI.h"
#include "Render/Resources/Buffer.h"
#include "Render/Resources/Mesh.h"
#include "Render/Resources/Pipeline.h"
#include "Render/Resources/Shader.h"
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

    void RenderCommand::Submit(const Drawable &drawable, const glm::mat4 &transform)
    {
        GetRenderAPI().Submit(drawable, transform);
    }

    void RenderCommand::UseCamera(const Camera &camera)
    {
        GetRenderAPI().UseCamera(camera);
    }
} // Sunset

/****************************************/
/* Shader                               */
/****************************************/

namespace Sunset
{
    std::unique_ptr<Shader> Shader::CreateShader(const std::string_view &vertShader, const std::string_view &fragShader)
    {
        return GetRenderAPI().CreateShader(vertShader, fragShader);
    }
}

/****************************************/
/* Buffer                               */
/****************************************/

namespace Sunset
{
    std::shared_ptr<Buffer> Buffer::Create(const BufferType& type)
    {
        return GetRenderAPI().CreateBuffer(type);
    }
}

/****************************************/
/* Texture                              */
/****************************************/

namespace Sunset
{
    std::unique_ptr<Texture> Texture::Create(const TextureDescription& desc)
    {
        return GetRenderAPI().CreateTexture(desc);
    }
}

/****************************************/
/* Pipeline                             */
/****************************************/

namespace Sunset
{
    std::shared_ptr<Pipeline> Pipeline::Create(const RenderState& state)
    {
        return GetRenderAPI().CreatePipeline(state);
    }
}

/****************************************/
/* Mesh                                 */
/****************************************/

namespace Sunset
{
    std::shared_ptr<Mesh> Mesh::CreateMesh(const void *data, const std::size_t typeSize, const std::size_t size, const VertexLayout &layout)
    {
        auto vertexBuffer = Buffer::Create(BufferType::Vertex);
        vertexBuffer->SetData(data, typeSize, size);
        std::shared_ptr<Buffer> indexBuffer = nullptr;
        return GetRenderAPI().CreateMesh(vertexBuffer, indexBuffer, layout);
    }
}
