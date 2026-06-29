//
// Created by sunvy on 28/06/2026.
//

#include "SRmGUI_Opengl.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "SRmGUI.h"
#include "Type.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace
{
    constexpr uint32_t MaxQuads = 10000;
    constexpr uint32_t VerticesPerQuad = 6;
    constexpr uint32_t MaxVertices = MaxQuads * VerticesPerQuad;

    unsigned int VAO = 0, VBO = 0, shaderProgram = 0;
    unsigned int WhiteTexture = 0;

    struct Vertex
    {
        glm::vec2 Position = {0, 0};
        glm::vec2 Uv = {0, 0};
        glm::vec4 Color = {1.f, 1.f, 1.f, 1.f};
    };

    std::vector<Vertex> Vertices;

    struct DrawCmd
    {
        SRmGUI::TextureID texture = 0;
        std::uint32_t VertexOffset = 0;
        std::uint32_t VertexCount = 0;
    };

    std::vector<DrawCmd> Cmds;

    const char* vertexShaderSource{R"(
        #version 330 core

        layout(location = 0) in vec2 a_Position;
        layout(location = 1) in vec2 a_Uv;
        layout(location = 2) in vec4 a_Color;

        uniform mat4 u_Projection;

        out vec2 v_Uv;
        out vec4 v_Color;

        void main()
        {
            v_Uv = a_Uv;
            v_Color = a_Color;
            gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0);
        }
    )"};

    const char* fragmentShaderSource{R"(
        #version 330 core

        in vec2 v_Uv;
        in vec4 v_Color;

        uniform sampler2D u_Texture;

        out vec4 FragColor;

        void main()
        {
            FragColor = texture(u_Texture, v_Uv) * v_Color;
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

    void AddDrawCommand(SRmGUI::TextureID texture, std::uint32_t vertexCount)
    {
        if (vertexCount == 0)
            return;

        if (!Cmds.empty() && Cmds.back().texture == texture)
        {
            Cmds.back().VertexCount += vertexCount;
            return;
        }

        DrawCmd& Cmd = Cmds.emplace_back();
        Cmd.texture = texture;
        Cmd.VertexOffset = static_cast<std::uint32_t>(Vertices.size()) - vertexCount;
        Cmd.VertexCount = vertexCount;
    }

    void PushTextureQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const glm::vec4& uv, SRmGUI::TextureID textureID)
    {
        if (Vertices.size() + VerticesPerQuad > MaxVertices)
            return;

        const std::uint32_t start = static_cast<std::uint32_t>(Vertices.size());

        const glm::vec2 p0 = pos;
        const glm::vec2 p1 = pos + glm::vec2{size.x, 0.f};
        const glm::vec2 p2 = pos + size;
        const glm::vec2 p3 = pos + glm::vec2{0.f, size.y};

        const glm::vec2 uv0 = {uv.x, uv.y};
        const glm::vec2 uv1 = {uv.z, uv.y};
        const glm::vec2 uv2 = {uv.z, uv.w};
        const glm::vec2 uv3 = {uv.x, uv.w};

        Vertices.emplace_back(Vertex{p0, uv0, color});
        Vertices.emplace_back(Vertex{p1, uv1, color});
        Vertices.emplace_back(Vertex{p2, uv2, color});

        Vertices.emplace_back(Vertex{p2, uv2, color});
        Vertices.emplace_back(Vertex{p3, uv3, color});
        Vertices.emplace_back(Vertex{p0, uv0, color});

        const std::uint32_t count =
            static_cast<std::uint32_t>(Vertices.size()) - start;

        AddDrawCommand(textureID, count);
    }

    void PushQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        if (Vertices.size() + VerticesPerQuad > MaxVertices)
            return;

        const std::uint32_t start = static_cast<std::uint32_t>(Vertices.size());

        Vertices.emplace_back(Vertex{pos + size                  , {}, color});
        Vertices.emplace_back(Vertex{pos + glm::vec2{size.x, 0}, {}, color});
        Vertices.emplace_back(Vertex{pos                         , {}, color});
        Vertices.emplace_back(Vertex{pos                         , {}, color});
        Vertices.emplace_back(Vertex{pos + glm::vec2{0, size.y}, {}, color});
        Vertices.emplace_back(Vertex{pos + size                  , {}, color});

        const std::uint32_t count = static_cast<std::uint32_t>(Vertices.size()) - start;
        AddDrawCommand(WhiteTexture, count);
    }

    void CreateWhiteTexture()
    {
        const unsigned char whitePixel[4] = {255, 255, 255, 255};

        glGenTextures(1, &WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, WhiteTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    struct Glyph
    {
        glm::vec2 UVMin;
        glm::vec2 UVMax;

        glm::vec2 Size;      // taille du quad en pixels
        glm::vec2 Bearing;   // offset depuis la baseline
        float Advance;       // déplacement du curseur en pixels
    };

    struct Font
    {
        uint32_t TextureID = 0;

        int AtlasWidth = 0;
        int AtlasHeight = 0;

        float PixelSize = 16.0f;

        std::array<Glyph, 128> Glyphs;

        ~Font()
        {
            glDeleteTextures(1, &TextureID);
        }
    };

    std::unique_ptr<Font> font = nullptr;

    std::unique_ptr<Font>& LoadFont(const std::string &path, float pixelSize)
    {
        constexpr int atlasWidth = 512;
        constexpr int atlasHeight = 512;
        constexpr int firstChar = 32;
        constexpr int charCount = 96;

        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
            throw std::runtime_error("Failed to open font file");

        const std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> fontBuffer(size);
        if (!file.read(reinterpret_cast<char*>(fontBuffer.data()), size))
            throw std::runtime_error("Failed to read font file");

        std::vector<unsigned char> bitmap(atlasWidth * atlasHeight);

        std::array<stbtt_bakedchar, charCount> bakedChars{};

        const int result = stbtt_BakeFontBitmap(
            fontBuffer.data(),
            0,
            pixelSize,
            bitmap.data(),
            atlasWidth,
            atlasHeight,
            firstChar,
            charCount,
            bakedChars.data()
        );

        if (result <= 0)
            throw std::runtime_error("Failed to bake font bitmap");

        std::unique_ptr<::Font> f = std::make_unique<::Font>();
        f->AtlasWidth = atlasWidth;
        f->AtlasHeight = atlasHeight;
        f->PixelSize = pixelSize;

        // Upload OpenGL
        glCreateTextures(GL_TEXTURE_2D, 1, &(f->TextureID));
        glTextureStorage2D(f->TextureID, 1, GL_R8, atlasWidth, atlasHeight);
        glTextureSubImage2D(
            f->TextureID,
            0,
            0,
            0,
            atlasWidth,
            atlasHeight,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bitmap.data()
        );

        glTextureParameteri(f->TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(f->TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(f->TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(f->TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLint swizzle[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
        glTextureParameteriv(f->TextureID, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

        for (int i = 0; i < charCount; ++i)
        {
            const stbtt_bakedchar& bc = bakedChars[i];

            Glyph glyph{};

            glyph.UVMin = {
                static_cast<float>(bc.x0) / static_cast<float>(atlasWidth),
                static_cast<float>(bc.y0) / static_cast<float>(atlasHeight)
            };

            glyph.UVMax = {
                static_cast<float>(bc.x1) / static_cast<float>(atlasWidth),
                static_cast<float>(bc.y1) / static_cast<float>(atlasHeight)
            };

            glyph.Size = {
                static_cast<float>(bc.x1 - bc.x0),
                static_cast<float>(bc.y1 - bc.y0)
            };

            glyph.Bearing = {
                bc.xoff,
                bc.yoff
            };

            glyph.Advance = bc.xadvance;

            f->Glyphs[firstChar + i] = glyph;
        }

        return font;
    }

    void PushText(const SRmGUI::Forme::Text& text)
    {
        glm::vec2 cursor = text.Position;

        // Si Position représente le top-left du texte :
        const float baselineY = text.Position.y + font->PixelSize;

        const std::uint32_t start = static_cast<std::uint32_t>(Vertices.size());

        for (char c : text.text)
        {
            if (c == '\n')
            {
                cursor.x = text.Position.x;
                cursor.y += font->PixelSize;
                continue;
            }

            const unsigned char uc = static_cast<unsigned char>(c);
            if (uc < 32 || uc >= 128)
                continue;

            const Glyph& glyph = font->Glyphs[uc];

            const glm::vec2 glyphPos = {
                cursor.x + glyph.Bearing.x,
                baselineY + (cursor.y - text.Position.y) + glyph.Bearing.y
            };

            const glm::vec2 glyphSize = glyph.Size;

            PushTextureQuad(
                glyphPos,
                glyphSize,
                text.Color,
                glm::vec4{
                    glyph.UVMin.x,
                    glyph.UVMin.y,
                    glyph.UVMax.x,
                    glyph.UVMax.y
                },
                font->TextureID
            );

            cursor.x += glyph.Advance;
        }
    }
}

namespace SRmGUI
{
    void Opengl_Init()
    {
        // Shader
        CreateShader(vertexShaderSource, fragmentShaderSource, shaderProgram);

        CreateWhiteTexture();

        font = std::move(LoadFont(SRmGUI_RESOURCES "Tiny5-Regular.ttf", 15));

        // Buffer
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uv));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

        glBindVertexArray(0);
    }

    void Opengl_Shutdown()
    {
        font.reset();
        glDeleteTextures(1, &WhiteTexture);
        glDeleteProgram(shaderProgram);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void Opengl_DrawData(const FormeDatas& formesData)
    {
        // Clear all
        Vertices.clear();
        Vertices.reserve(MaxVertices);
        Cmds.clear();

        // Emplace all vertices & DrawCmds
        for (const auto& form : formesData)
        {
            std::visit(overloads{
                [&](const Forme::Rectangle& rect)
                {
                    PushQuad(rect.Position, rect.Size, rect.Color);
                },
                [&](const Forme::Image& image)
                {
                    PushTextureQuad(image.Position, image.Size, image.Color, image.Uv, image.texture);
                },
                [&](const Forme::Text& text)
                {
                    PushText(text);
                }
            }, form);
        }

        if (Vertices.empty())
            return;

        const auto& windowSize = SRmGUI::GetContext().GetWindowSize();
        const glm::mat4 projection = glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f);

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_Projection"), 1, GL_FALSE, &projection[0][0]);

        glUniform1i(glGetUniformLocation(shaderProgram, "u_Texture"), 0);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(Vertices.size() * sizeof(Vertex)), Vertices.data());

        glActiveTexture(GL_TEXTURE0);

        for (const DrawCmd& cmd : Cmds)
        {
            glBindTexture(GL_TEXTURE_2D, cmd.texture);

            glDrawArrays(
                GL_TRIANGLES,
                static_cast<GLint>(cmd.VertexOffset),
                static_cast<GLsizei>(cmd.VertexCount)
            );
        }

        glBindVertexArray(0);
    }
}
