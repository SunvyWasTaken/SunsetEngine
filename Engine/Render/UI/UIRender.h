//
// Created by sunvy on 24/06/2026.
//

#pragma once

namespace Sunset
{
    class Texture;
    class UIRenderList;
    class Shader;

    struct UIVertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
        glm::vec4 Color;
        float TextureIndex = 0.0f;
    };

    class UIRender
    {
    public:
        UIRender();
        virtual ~UIRender();

        void Render(const UIRenderList& RenderList);
    private:
        void PushQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const glm::vec4& uv, float textureIndex = 0.0f);
        void PushText(const std::string& text, const glm::vec2& pos, const glm::vec2& glyphSize, const glm::vec4& color);
    private:
        static constexpr uint32_t MaxTextureSlots = 16;
        uint32_t VBO = 0, VAO = 0, EBO = 0;
        std::vector<UIVertex> Vertices;
        std::unique_ptr<Shader> m_Shader;
    };
} // Sunset
