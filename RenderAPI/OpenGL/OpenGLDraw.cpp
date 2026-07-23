//
// Created by Codex on 23/07/2026.
//

#include "OpenGLDraw.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLPrimitiveType(const Sunset::PrimitiveType type)
    {
        switch (type)
        {
            case Sunset::PrimitiveType::Points:        return GL_POINTS;
            case Sunset::PrimitiveType::Lines:         return GL_LINES;
            case Sunset::PrimitiveType::LineStrip:     return GL_LINE_STRIP;
            case Sunset::PrimitiveType::Triangles:     return GL_TRIANGLES;
            case Sunset::PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
            case Sunset::PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
        }

        return GL_POINTS;
    }
}

namespace Sunset::OpenGLDraw
{
    void Draw(const std::uint32_t vertexCount, const RenderState& state)
    {
        if (state.DrawInstance)
            glDrawArraysInstanced(ToGLPrimitiveType(state.primitiveType), 0, state.nbrInstance, vertexCount);
        else if (state.HasIndice)
            glDrawElements(ToGLPrimitiveType(state.primitiveType), vertexCount, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(ToGLPrimitiveType(state.primitiveType), 0, vertexCount);
    }
}
