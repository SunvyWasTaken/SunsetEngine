//
// Created by sunvy on 26/08/2026.
//

#include "OpenGLPipeline.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLBlendFactor(const Sunset::BlendFactor factor)
    {
        switch (factor)
        {
            case Sunset::BlendFactor::Zero:                return GL_ZERO;
            case Sunset::BlendFactor::One:                 return GL_ONE;
            case Sunset::BlendFactor::SrcAlpha:            return GL_SRC_ALPHA;
            case Sunset::BlendFactor::OneMinusSrcAlpha:    return GL_ONE_MINUS_SRC_ALPHA;
            case Sunset::BlendFactor::DstAlpha:            return GL_DST_ALPHA;
            case Sunset::BlendFactor::OneMinusDstAlpha:    return GL_ONE_MINUS_DST_ALPHA;
            case Sunset::BlendFactor::SrcColor:            return GL_SRC_COLOR;
            case Sunset::BlendFactor::OneMinusSrcColor:    return GL_ONE_MINUS_SRC_COLOR;
            case Sunset::BlendFactor::DstColor:            return GL_DST_COLOR;
            case Sunset::BlendFactor::OneMinusDstColor:    return GL_ONE_MINUS_DST_COLOR;
        }

        return GL_ONE;
    }

    GLenum ToGLCullMode(const Sunset::CullMode mode)
    {
        switch (mode)
        {
            case Sunset::CullMode::Back:  return GL_BACK;
            case Sunset::CullMode::Front: return GL_FRONT;
            case Sunset::CullMode::None:  return GL_BACK;
        }

        return GL_BACK;
    }
}

namespace Sunset
{
    OpenGLPipeline::OpenGLPipeline(const RenderState &state)
        : m_RenderState(state)
    {
    }

    OpenGLPipeline::~OpenGLPipeline()
    {
    }

    void OpenGLPipeline::Bind()
    {
        m_RenderState.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glDepthMask(m_RenderState.depthWrite ? GL_TRUE : GL_FALSE);

        if (m_RenderState.blending)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(ToGLBlendFactor(m_RenderState.src), ToGLBlendFactor(m_RenderState.dest), GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (m_RenderState.cullMode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(ToGLCullMode(m_RenderState.cullMode));
        }

        glPolygonMode(GL_FRONT_AND_BACK, m_RenderState.wireframe ? GL_LINE : GL_FILL);
    }
} // Sunset