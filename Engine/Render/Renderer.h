//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_RENDERER_H
#define SUNSETCRAFT_RENDERER_H
#include "Core/Input.h"

namespace SunsetEngine
{
    class Renderer final
    {
    public:
        Renderer();
        ~Renderer();

        void BindEvent(std::function<void(Event::Type&)> func);

        bool Valid() const;

        static void* Get();
    };
}

#endif //SUNSETCRAFT_RENDERER_H