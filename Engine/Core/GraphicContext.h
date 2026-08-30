//
// Created by sunvy on 30/08/2026.
//

#ifndef SUNSETENGINE_GRAPHICCONTEXT_H
#define SUNSETENGINE_GRAPHICCONTEXT_H

namespace Sunset
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}

#endif //SUNSETENGINE_GRAPHICCONTEXT_H