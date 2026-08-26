//
// Created by sunvy on 19/12/2025.
//

#pragma once

namespace Sunset
{
    class Shader;
    struct Image;

    class Texture
    {
    public:

        virtual ~Texture() = default;

        virtual void Bind() const = 0;
    };
} // Sunset