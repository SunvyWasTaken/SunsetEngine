//
// Created by sunvy on 28/06/2026.
//

#include "SRmGUI_Opengl.h"

#include <glad/glad.h>

#include "Sources/Type.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;

    unsigned int VAO = 0, VBO = 0, shaderProgram = 0;

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec4 Color;
    };

    std::vector<Vertex> Vertices;

    const char* vertexShaderSource{" \
        #version 330 core \
        \
        layout(location = 0) in vec2 a_Position; \
        layout(location = 1) in vec2 a_TexCoord; \
        layout(location = 2) in vec4 a_Color; \
        layout(location = 3) in float a_TextureIndex; \
        \
        uniform mat4 u_Projection; \
        \
        out vec2 v_TexCoord; \
        out vec4 v_Color; \
        out float v_TextureIndex; \
        \
        void main() \
        { \
            v_TexCoord = a_TexCoord; \
            v_Color = a_Color; \
            v_TextureIndex = a_TextureIndex; \
            gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0); \
        }"
    };

    const char* fragmentShaderSource{""
        "#version 330 core \
        \
        in vec2 v_TexCoord;\
        in vec4 v_Color; \
        out vec4 FragColor; \
        \
        void main()\
        { \
            FragColor = v_Color; \
        }"
    };

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
    }

    void Opengl_Shutdown()
    {
        glDeleteProgram(shaderProgram);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void Opengl_DrawData(const FormeDatas& formesData)
    {
        for (const auto& form : formesData)
        {
            std::visit(overloads{
                [&](const Forme::Rectangle& rect)
                {

                },
                [&](const Forme::Image& image)
                {

                },
                [&](const Forme::Text& text)
                {

                }
            }, form);
        }
    }
}
