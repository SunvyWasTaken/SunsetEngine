//
// Created by sunvy on 24/06/2026.
//

#pragma once

namespace Sunset
{
    class UIRenderList;
    class Shader;

    struct UIVertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
        glm::vec4 Color;
        uint32_t TextureIndex;
    };

    class UIRender
    {
    public:
        UIRender();
        virtual ~UIRender();

        void Render(const UIRenderList& RenderList);
    private:
        void PushQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, uint32_t textureIndex, const glm::vec4& uv);
        uint32_t GetTextureSlot(uint32_t textureId, std::vector<uint32_t>& textureSlots) const;
    private:
        static constexpr uint32_t MaxTextureSlots = 16;
        uint32_t VBO = 0, VAO = 0, EBO = 0;
        uint32_t WhiteTexture = 0;
        std::vector<UIVertex> Vertices;
        std::unique_ptr<Shader> m_Shader;
    };
} // Sunset
