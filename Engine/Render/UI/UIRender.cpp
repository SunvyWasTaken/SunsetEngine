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

        constexpr uint32_t whitePixel = 0xffffffff;
        glGenTextures(1, &WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, WhiteTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

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
        glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(UIVertex), (void*)offsetof(UIVertex, TextureIndex));

        glBindVertexArray(0);
    }

    UIRender::~UIRender()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteTextures(1, &WhiteTexture);
    }

    void UIRender::Render(const UIRenderList &RenderList)
    {
        Vertices.clear();
        std::vector<uint32_t> textureSlots;
        textureSlots.reserve(MaxTextureSlots);
        textureSlots.emplace_back(WhiteTexture);

        if (RenderList.IsEmpty())
            return;

        for (const UIDraw::Type& draw : RenderList.m_UIDrawList)
        {
            std::visit(overloads{
                [&](const UIDraw::Rectangle& rect)
                {
                    PushQuad(rect.Position, rect.Size, rect.Color, 0, {0, 0, 1, 1});
                },
                [&](const UIDraw::Image& img)
                {
                    PushQuad(img.Position, img.Size, img.Color, GetTextureSlot(img.TextureId, textureSlots), img.Uv);
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_Shader->Use();
        m_Shader->SetMat4("u_Projection", projection);
        for (uint32_t i = 0; i < MaxTextureSlots; ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, i < textureSlots.size() ? textureSlots[i] : WhiteTexture);
            m_Shader->SetInt(std::format("u_Textures[{}]", i), static_cast<int>(i));
        }

        glBindVertexArray(VAO);
        glNamedBufferSubData(VBO, 0, static_cast<GLsizeiptr>(Vertices.size() * sizeof(UIVertex)), Vertices.data());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((Vertices.size() / 4) * 6), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    void UIRender::PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color, uint32_t textureIndex,
        const glm::vec4 &uv)
    {
        if (Vertices.size() + 4 > MaxVertices)
            return;

        Vertices.emplace_back(UIVertex{pos                         , glm::vec2{uv.x, uv.y}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{size.x, 0}, glm::vec2{uv.z, uv.y}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + size                  , glm::vec2{uv.z, uv.w}, color, textureIndex});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{0, size.y}, glm::vec2{uv.x, uv.w}, color, textureIndex});
    }

    uint32_t UIRender::GetTextureSlot(uint32_t textureId, std::vector<uint32_t>& textureSlots) const
    {
        if (textureId == 0)
            return 0;

        for (uint32_t i = 1; i < textureSlots.size(); ++i)
            if (textureSlots[i] == textureId)
                return i;

        if (textureSlots.size() >= MaxTextureSlots)
            return 0;

        textureSlots.emplace_back(textureId);
        return static_cast<uint32_t>(textureSlots.size() - 1);
    }
} // Sunset
