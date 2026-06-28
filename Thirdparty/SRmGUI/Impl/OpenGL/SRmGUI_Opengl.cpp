//
// Created by sunvy on 28/06/2026.
//

#include "SRmGUI_Opengl.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "Sources/SRmGUI.h"
#include "Sources/Type.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t VerticesPerQuad = 6;
    constexpr uint32_t MaxVertices = MaxQuads * VerticesPerQuad;

    unsigned int VAO = 0, VBO = 0, shaderProgram = 0;

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec4 Color;
    };

    std::vector<Vertex> Vertices;

    const char* vertexShaderSource{R"(
        #version 330 core;

        layout(location = 0) in vec2 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_Projection;

        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0);
        }
    )"};

    const char* fragmentShaderSource{R"(
        #version 330 core

        in vec4 v_Color;
        out vec4 FragColor;

        void main()
        {
            FragColor = v_Color;
        }
    )"};

    void CreateShader(const char * vertexShaderSource, const char * fragmentShaderSource, unsigned int& shaderProgram)
    {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);


        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        if (Vertices.size() + VerticesPerQuad > MaxVertices)
            return;

        Vertices.emplace_back(Vertex{pos + size                  , color});
        Vertices.emplace_back(Vertex{pos + glm::vec2{size.x, 0}, color});
        Vertices.emplace_back(Vertex{pos                         , color});
        Vertices.emplace_back(Vertex{pos                         , color});
        Vertices.emplace_back(Vertex{pos + glm::vec2{0, size.y}, color});
        Vertices.emplace_back(Vertex{pos + size                  , color});
    }
}

namespace SRmGUI
{
    void Opengl_Init()
    {
        // Shader
        CreateShader(vertexShaderSource, fragmentShaderSource, shaderProgram);

        // Buffer
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

        glBindVertexArray(0);
    }

    void Opengl_Shutdown()
    {
        glDeleteProgram(shaderProgram);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void Opengl_DrawData(const FormeDatas& formesData)
    {
        Vertices.clear();
        Vertices.reserve(MaxVertices);

        for (const auto& form : formesData)
        {
            std::visit(overloads{
                [&](const Forme::Rectangle& rect)
                {
                    PushQuad(rect.Position, rect.Size, rect.Color);
                },
                [&](const Forme::Image& image)
                {

                },
                [&](const Forme::Text& text)
                {

                }
            }, form);
        }

        if (Vertices.empty())
            return;

        const auto& windowSize = SRmGUI::GetContext().GetWindowSize();
        const glm::mat4 projection = glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_Projection"), 1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(Vertices.size() * sizeof(Vertex)), Vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Vertices.size()));

        glBindVertexArray(0);
    }
}
