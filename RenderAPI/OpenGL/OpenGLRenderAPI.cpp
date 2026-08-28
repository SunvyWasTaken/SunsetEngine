//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLDrawQueue.h"
#include "Resources/OpenGLRenderTarget.h"
#include "Resources/OpenGLBuffer.h"
#include "Resources/OpenGLMesh.h"
#include "Resources/OpenGLPipeline.h"
#include "Resources/OpenGLShader.h"
#include "Resources/OpenGLTexture.h"

namespace
{

}

namespace Sunset
{
    struct OpenGLRenderAPI::OpenGLData
    {
        OpenGLDrawQueue m_DrawQueue;
    };

    OpenGLRenderAPI::OpenGLRenderAPI()
    {
        INITLOG("OpenGL")
    }

    OpenGLRenderAPI::~OpenGLRenderAPI()
    {
        LOG("OpenGL", info, "OpenGL Render API is Destroy")
    }

    void OpenGLRenderAPI::Init()
    {
        LOG("OpenGL", info, "OpenGLRenderAPI init")
        const int loader = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        if (loader == GL_FALSE)
            throw std::runtime_error("Failed to initialize GLAD");

        data = std::make_unique<OpenGLData>();
    }

    void OpenGLRenderAPI::Clear(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::BeginFrame()
    {
        Clear({0, 0, 0, 0});
    }

    void OpenGLRenderAPI::EndFrame()
    {
        data->m_DrawQueue.Flush();
    }

    void OpenGLRenderAPI::Submit(const Drawable &drawable, const glm::mat4 &transform)
    {
        data->m_DrawQueue.Submit(drawable, transform);
    }

    void OpenGLRenderAPI::UseCamera(const Camera &camera)
    {
        data->m_DrawQueue.UseCamera(camera);
    }

    void OpenGLRenderAPI::SetViewport(const glm::ivec2 &screenSize)
    {
        glViewport(0, 0, screenSize.x, screenSize.y);
    }
}

/*--------------------------------------*/
/* Shader                               */
/*--------------------------------------*/

namespace Sunset
{
    std::unique_ptr<Shader> OpenGLRenderAPI::CreateShader(const std::string_view&vertShader, const std::string_view&fragShader)
    {
        return std::make_unique<OpenGLShader>(vertShader, fragShader);
    }
}

/****************************************/
/* Buffer                               */
/****************************************/

namespace Sunset
{
    std::shared_ptr<Buffer> OpenGLRenderAPI::CreateBuffer(const BufferType& type)
    {
        return std::make_shared<OpenGLBuffer>(type);
    }
}

/****************************************/
/* Texture                              */
/****************************************/

namespace Sunset
{
    std::unique_ptr<Texture> OpenGLRenderAPI::CreateTexture(const TextureDescription &desc)
    {
        return std::make_unique<OpenGLTexture>(desc);
    }
} // Sunset

/****************************************/
/* Pipeline                             */
/****************************************/

namespace Sunset
{
    std::shared_ptr<Pipeline> OpenGLRenderAPI::CreatePipeline(const RenderState &state)
    {
        return std::make_shared<OpenGLPipeline>(state);
    }

    std::shared_ptr<Mesh> OpenGLRenderAPI::CreateMesh(const std::shared_ptr<Buffer> &vertexBuffer,
        const std::shared_ptr<Buffer> &indexBuffer, const VertexLayout &vertexLayout)
    {
        return std::make_shared<OpenGLMesh>(vertexBuffer, indexBuffer, vertexLayout);
    }

    std::shared_ptr<RenderTarget> OpenGLRenderAPI::CreateFramebuffer(const RenderTargetSpecification &spec)
    {
        return std::make_shared<OpenGLRenderTarget>(spec);
    }
}
