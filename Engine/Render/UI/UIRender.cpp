//
// Created by sunvy on 24/06/2026.
//

#include "UIRender.h"

#include <glad/glad.h>

#include "UIRenderList.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;
}

namespace Sunset
{
    UIRender::UIRender()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glNamedBufferData(
            VBO,
            MaxVertices * sizeof(UIVertex),
            nullptr,
            GL_DYNAMIC_DRAW
        );

        std::vector<uint32_t> indices;
        indices.reserve(MaxVertices);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < MaxQuads; ++i)
        {
            indices.emplace_back(offset + 0);
            indices.emplace_back(offset + 1);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 3);
            indices.emplace_back(offset + 0);
            offset += 4;
        }

        glNamedBufferData(
            EBO,
            indices.size() * sizeof(uint32_t),
            indices.data(),
            GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, TexCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Color));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, TextureIndex));
    }

    UIRender::~UIRender()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void UIRender::Render(const UIRenderList &RenderList)
    {
        Vertices.clear();

        for (const UIDraw::Type& draw : RenderList.m_UIDrawList)
        {
            std::visit(overloads{
                [&](const UIDraw::Rectangle& rect)
                {
                    PushQuad(rect.Position, rect.Size, rect.Color, 0, {0, 0, 1, 1});
                },
                [&](const UIDraw::Image& img)
                {
                    PushQuad(img.Position, img.Size, img.Color, img.TextureId, img.Uv);
                },
                [&](const UIDraw::Text& text)
                {

                }
            }, draw);
        }
    }

    void UIRender::PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color, uint32_t textureIndex,
        const glm::vec4 &uv)
    {
        Vertices.emplace_back(UIVertex{pos                         , glm::vec2{uv.x, uv.y}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{size.x, 0}, glm::vec2{uv.z, uv.y}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + size                  , glm::vec2{uv.z, uv.w}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{0, size.y}, glm::vec2{uv.x, uv.w}, color, textureIndex});
    }
} // Sunset