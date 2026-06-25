//
// Created by sunvy on 24/06/2026.
//

#include "UIRender.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Render/Shader.h"
#include "UIRenderList.h"
#include "Render/Texture.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;
}

namespace Sunset
{
    UIRender::UIRender()
    {
        m_Shader = std::make_unique<Shader>(ENGINE_SHADERS_PATH "UI.vert", ENGINE_SHADERS_PATH "UI.frag");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            MaxVertices * sizeof(UIVertex),
            nullptr,
            GL_DYNAMIC_DRAW);

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

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint32_t),
            indices.data(),
        GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, TexCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Color));

        glBindVertexArray(0);
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
        std::vector<std::shared_ptr<Texture>> textureSlots;
        textureSlots.reserve(MaxTextureSlots);

        if (RenderList.IsEmpty())
            return;

        for (const UIDraw::Type& draw : RenderList.m_UIDrawList)
        {
            std::visit(overloads{
                [&](const UIDraw::Rectangle& rect)
                {
                    PushQuad(rect.Position, rect.Size, rect.Color, {0, 0, 1, 1});
                },
                [&](const UIDraw::Image& img)
                {
                    PushQuad(img.Position, img.Size, img.Color, img.Uv);
                },
                [&](const UIDraw::Text& text)
                {

                }
            }, draw);
        }

        if (Vertices.empty())
            return;

        const auto& setting = Application::GetSetting();
        const glm::mat4 projection = glm::ortho(
            0.0f,
            static_cast<float>(setting.WindowSize.x),
            static_cast<float>(setting.WindowSize.y),
            0.0f,
            -1.0f,
            1.0f);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_Shader->Use();
        m_Shader->SetMat4("u_Projection", projection);
        size_t i = 0;
        for (const auto& texture : textureSlots)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            texture->Use();
            m_Shader->SetInt(std::format("u_Textures[{}]", i), static_cast<int>(i));
            ++i;
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(Vertices.size() * sizeof(UIVertex)), Vertices.data());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((Vertices.size() / 4) * 6), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    void UIRender::PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color, const glm::vec4 &uv)
    {
        if (Vertices.size() + 4 > MaxVertices)
            return;

        Vertices.emplace_back(UIVertex{pos                         , glm::vec2{uv.x, uv.y}, color});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{size.x, 0}, glm::vec2{uv.z, uv.y}, color});
        Vertices.emplace_back(UIVertex{pos + size                  , glm::vec2{uv.z, uv.w}, color});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{0, size.y}, glm::vec2{uv.x, uv.w}, color});
    }
} // Sunset
