//
// Created by sunvy on 14/06/2026.
//

#pragma once

#include "Render/Core/RenderHandle.h"

namespace Sunset
{
    template <typename T>
    struct UniformBuffer final
    {
        UniformBuffer() = default;

        ~UniformBuffer();

        void Init(size_t size);

        void Update(const std::vector<T>& data) const;
    private:
        BufferHandle m_Id;
        size_t m_Size = 0;
    };
} // Sunset
