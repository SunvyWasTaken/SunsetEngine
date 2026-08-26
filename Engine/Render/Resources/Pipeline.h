//
// Created by sunvy on 26/08/2026.
//

#pragma once

namespace Sunset
{
    struct RenderState;

    class Pipeline
    {
    public:
        static std::shared_ptr<Pipeline> Create(const RenderState& state);
    public:
        virtual ~Pipeline() = default;
        virtual void Bind() = 0;
    };
} // Sunset