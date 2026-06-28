//
// Created by sunvy on 24/06/2026.
//

#include "UIRender.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <algorithm>
#include <iterator>

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Render/Shader.h"
#include "UIRenderList.h"
#include "Render/Texture.h"

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;

    std::array<uint8_t, 7> GlyphRows(char character)
        {
            switch (static_cast<char>(std::toupper(static_cast<unsigned char>(character))))
            {
                case 'A': return {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
                case 'B': return {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E};
                case 'C': return {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F};
                case 'D': return {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E};
                case 'E': return {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F};
                case 'F': return {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10};
                case 'G': return {0x0F, 0x10, 0x10, 0x13, 0x11, 0x11, 0x0F};
                case 'H': return {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
                case 'I': return {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F};
                case 'J': return {0x01, 0x01, 0x01, 0x01, 0x11, 0x11, 0x0E};
                case 'K': return {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11};
                case 'L': return {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};
                case 'M': return {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11};
                case 'N': return {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11};
                case 'O': return {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
                case 'P': return {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10};
                case 'Q': return {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D};
                case 'R': return {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};
                case 'S': return {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E};
                case 'T': return {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
                case 'U': return {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
                case 'V': return {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04};
                case 'W': return {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A};
                case 'X': return {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11};
                case 'Y': return {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04};
                case 'Z': return {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F};
                case '0': return {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E};
                case '1': return {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E};
                case '2': return {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F};
                case '3': return {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E};
                case '4': return {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02};
                case '5': return {0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E};
                case '6': return {0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E};
                case '7': return {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08};
                case '8': return {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E};
                case '9': return {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E};
                case '.': return {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C};
                case ',': return {0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08};
                case '!': return {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04};
                case '?': return {0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04};
                case ':': return {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00};
                case '-': return {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00};
                case '+': return {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00};
                case '/': return {0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10};
                case ' ': return {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
                default: return {0x1F, 0x11, 0x15, 0x15, 0x15, 0x11, 0x1F};
            }
        }
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
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, TextureIndex));

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
        Vertices.reserve(MaxVertices);

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
                    PushQuad(img.Position, img.Size, img.Color, img.Uv, img.m_texture);
                },
                [&](const UIDraw::Text& text)
                {
                    PushText(text.text, text.Position, text.Size, text.Color);
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



        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(Vertices.size() * sizeof(UIVertex)), Vertices.data());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((Vertices.size() / 4) * 6), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    void UIRender::PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color, const glm::vec4 &uv, const std::shared_ptr<Texture>& texture)
    {
        if (Vertices.size() + 4 > MaxVertices)
            return;

        Vertices.emplace_back(UIVertex{pos                         , glm::vec2{uv.x, uv.y}, color, texture});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{size.x, 0}, glm::vec2{uv.z, uv.y}, color, texture});
        Vertices.emplace_back(UIVertex{pos + size                  , glm::vec2{uv.z, uv.w}, color, texture});
        Vertices.emplace_back(UIVertex{pos + glm::vec2{0, size.y}, glm::vec2{uv.x, uv.w}, color, texture});
    }

    void UIRender::PushText(const std::string& text, const glm::vec2& pos, const glm::vec2& glyphSize, const glm::vec4& color)
    {
        if (text.empty())
            return;

        const glm::vec2 pixelSize = glm::max(glyphSize / glm::vec2{5.0f, 7.0f}, glm::vec2{1.0f, 1.0f});
        const float advance = pixelSize.x * 6.0f;
        glm::vec2 cursor = pos;

        for (const char character : text)
        {
            if (character == '\n')
            {
                cursor.x = pos.x;
                cursor.y += pixelSize.y * 8.0f;
                continue;
            }

            const std::array<uint8_t, 7> rows = GlyphRows(character);
            for (uint32_t y = 0; y < rows.size(); ++y)
            {
                for (uint32_t x = 0; x < 5; ++x)
                {
                    if ((rows[y] & (1u << (4u - x))) != 0u)
                    {
                        PushQuad(cursor + glm::vec2{x * pixelSize.x, y * pixelSize.y}, pixelSize, color);
                    }
                }
            }

            cursor.x += advance;
        }
    }
} // Sunset
