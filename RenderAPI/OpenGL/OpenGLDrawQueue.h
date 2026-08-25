//
// Created by sunvy on 25/08/2026.
//

#pragma once

namespace Sunset
{
    struct Camera;

    class OpenGLDrawQueue
    {
    public:
        void Submit();
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

        };

        FrameData m_FrameData;
        std::vector<DrawCommand> m_DrawCommands;
    };
} // Sunset