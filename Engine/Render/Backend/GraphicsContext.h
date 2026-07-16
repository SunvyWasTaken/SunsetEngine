//
// Created by sunvy on 16/07/2026.
//

#pragma once

namespace Sunset
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void MakeCurrent() = 0;
        virtual void SwapBuffers() = 0;
    };
} // Sunset