//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Core/Event.h"

namespace Sunset
{
    class Render final
    {
    public:
        Render();
        ~Render();

        void BindEvent(std::function<void(Event::Type&)> func);

        bool Valid() const;

        static void* Get();
    };
}
