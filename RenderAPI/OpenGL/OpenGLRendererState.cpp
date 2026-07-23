//
// Created by Codex on 23/07/2026.
//

#include "OpenGLRendererState.h"

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

namespace Sunset::OpenGLRendererState
{
    void Reset()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Apply(const RenderState& state)
    {
        state.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glDepthMask(state.depthWrite ? GL_TRUE : GL_FALSE);

        if (state.blending)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(ToGLBlendFactor(state.src), ToGLBlendFactor(state.dest), GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (state.cullMode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(ToGLCullMode(state.cullMode));
        }

        glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
    }

    void SetCullMode(const CullMode mode)
    {
        if (mode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
            return;
        }

        glEnable(GL_CULL_FACE);
        glCullFace(ToGLCullMode(mode));
    }
}
