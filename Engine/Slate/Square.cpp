//
// Created by sunvy on 11/01/2026.
//

#include "Square.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Render/Shader.h"

#include <glm/gtc/constants.hpp>

#include "Render/Buffers.h"
#include "Render/Drawable.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "Render/VertexArray.h"

namespace
{
    constexpr int NbrPoints = 32;

    std::weak_ptr<SunsetEngine::Shader> _shader;
}

namespace SunsetEngine
{
    Square::Square(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec4& color, int radius)
        : m_Color(color)
        , m_Radius(radius)
        , m_Child(nullptr)
    {
        Slate::SetPosition(pos);
        Slate::SetSize(size);

        m_Drawable = std::make_unique<Drawable>();

        if (_shader.expired())
        {
            m_Drawable->m_Material->m_Shader =std::make_shared<Shader>("Engine/Shaders/Square.vert", "Engine/Shaders/Square.frag");
            _shader = m_Drawable->m_Material->m_Shader;
        }
        else
            m_Drawable->m_Material->m_Shader = _shader.lock();

        m_Drawable->m_Material->Set<glm::vec2>("u_ScreenSize", Application::GetSetting().WindowSize);
        m_Drawable->m_Material->Set<glm::vec4>("u_Color", m_Color);

        m_Drawable->m_RenderState.depthTest = false;
        m_Drawable->m_RenderState.depthWrite = true;
        // m_Drawable->m_RenderState.blending = true;
        // m_Drawable->m_RenderState.src = BlendFactor::SrcAlpha;
        // m_Drawable->m_RenderState.dest = BlendFactor::OneMinusSrcAlpha;
        m_Drawable->m_RenderState.cullMode = CullMode::None;
        m_Drawable->m_RenderState.primitiveType = PrimitiveType::TriangleFan;
        m_Drawable->m_RenderState.HasIndice = false;
    }

    Square::~Square()
    {
        Clear();
    }

    void Square::Draw() const
    {
        const_cast<Square*>(this)->Rebuild();

        Slate::Draw();

        if (m_Child)
            m_Child->Draw();
    }

    void Square::SetAnchor(const glm::vec2 &val)
    {
        m_Anchor = val;
        bIsDirty = true;
    }

    void Square::AddChild(const std::shared_ptr<Slate> &child)
    {
        m_Child = child;
        m_Child->SetPosition(m_Position);
        m_Child->SetSize(m_Size - glm::ivec2{m_Radius + 5, m_Radius + 5});
    }

    void Square::SetPosition(const glm::ivec2 &pos)
    {
        Slate::SetPosition(pos);
        if (m_Child)
            m_Child->SetPosition(pos);
    }

    void Square::SetSize(const glm::ivec2 &size)
    {
        Slate::SetSize(size);
        if (m_Child)
            m_Child->SetSize(size - glm::ivec2{m_Radius + 5, m_Radius + 5});
    }

    void Square::SetRadius(int radius)
    {
        m_Radius = radius;
        bIsDirty = true;
    }

    void Square::Clear()
    {
        m_Drawable->m_Mesh.reset();
    }

    void Square::Rebuild()
    {
        if (!bIsDirty)
            return;

        Clear();

        std::vector<glm::vec2> points;
        ComputePoints(points);

        auto VBO = std::make_shared<VertexBuffer>(points.data(), points.size(), sizeof(glm::vec2));

        VBO->SetLayout({BufferElement{ShaderDataType::Float2, "position"}});

        auto VAO = std::make_unique<VertexArray>();

        VAO->AddVertexBuffer(VBO);

        m_Drawable->m_Mesh = std::make_unique<Mesh>(VAO);
        m_Drawable->m_Mesh->m_VertexBuffer = VBO;

        bIsDirty = false;
    }

    void Square::ComputePoints(std::vector<glm::vec2> &points)
    {
        constexpr float TwoPi = glm::two_pi<float>();

        points.clear();
        points.reserve(NbrPoints);

        constexpr float step = TwoPi / NbrPoints;

        constexpr int Nbr = NbrPoints / 4;

        const int HalfSizeX = (m_Size.x / 2);
        const int HalfSizeY = (m_Size.y / 2);

        const std::array<glm::ivec2, 4> offsetPos
        {
            glm::ivec2{ m_Position.x + (HalfSizeX * (1 + m_Anchor.x)) - m_Radius, m_Position.y + (HalfSizeY * (1 + m_Anchor.y)) - m_Radius},
            glm::ivec2{ m_Position.x - (HalfSizeX * (1 - m_Anchor.x)) + m_Radius, m_Position.y + (HalfSizeY * (1 + m_Anchor.y)) - m_Radius},
            glm::ivec2{ m_Position.x - (HalfSizeX * (1 - m_Anchor.x)) + m_Radius, m_Position.y - (HalfSizeY * (1 - m_Anchor.y)) + m_Radius},
            glm::ivec2{ m_Position.x + (HalfSizeX * (1 + m_Anchor.x)) - m_Radius, m_Position.y - (HalfSizeY * (1 - m_Anchor.y)) + m_Radius}
        };


        int currentSide = 0;
        for (int i = 0; i < NbrPoints; ++i)
        {
            if (i >= currentSide * Nbr)
                ++currentSide;

            const glm::ivec2& currentOffset = offsetPos[currentSide - 1];
            const float xi = static_cast<float>(m_Radius) * glm::cos(step * i);
            const float yi = static_cast<float>(m_Radius) * glm::sin(step * i);
            points.emplace_back(currentOffset.x + xi, currentOffset.y + yi);
        }
    }
}
