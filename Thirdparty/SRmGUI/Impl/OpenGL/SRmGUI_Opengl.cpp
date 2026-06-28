//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "SRmGUI_Opengl.h"

#include <glad/glad.h>

#include "Sources/Type.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;

    unsigned int VAO = 0, VBO = 0;

    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };
}

namespace SRmGUI
{
    void Opengl_Init()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void Opengl_Shutdown()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void Opengl_DrawData(const FormeDatas& formesData)
    {
        for (const auto& form : formesData)
        {
            std::visit(::overloads{
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
