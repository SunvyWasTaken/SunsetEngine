//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderType.h"

#include <memory>
#include <vector>

namespace Sunset
{
    class Camera;
    class Drawable;
    class Material;
}

namespace Sunset
{
    class OpenGLDrawQueue
    {
    public:
        void Submit(const Drawable& drawable, const glm::mat4& model);
        void UseCamera(const Camera& camera);
        void Flush();

    private:
        struct FrameData
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
        };

        struct DrawCommand
        {
            std::uint32_t vertexArray = 0;
            std::uint32_t indexCount = 0;
            std::shared_ptr<Material> material;
            glm::mat4 model = glm::mat4(1.0f);
            RenderState state;
        };

        void Sort();

        FrameData m_FrameData;
        std::vector<DrawCommand> m_DrawCommands;
    };
}
