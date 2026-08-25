//
// Created by sunvy on 25/08/2026.
//

#pragma once
#include "Render/Core/RenderHandle.h"
#include "Render/Core/RenderType.h"

namespace Sunset
{
    struct Camera;
    class Drawable;
    class Material;

    class OpenGLDrawQueue
    {
    public:
        void Submit(const Drawable& drawable, const glm::mat4& model);
        void UseCamera(const Camera& camera);
        void Flush();
    private:

        void Sort();

        struct FrameData
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
        };

        struct DrawCommand
        {
            VertexArrayHandle vertexArray;
            std::uint32_t indexCount = 0;
            std::shared_ptr<Material> material;
            glm::mat4 model = glm::mat4(1.0f);
            RenderState state;
        };

        FrameData m_FrameData;
        std::vector<DrawCommand> m_DrawCommands;
    };
} // Sunset