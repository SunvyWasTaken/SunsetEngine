//
// Created by sunvy on 04/07/2026.
//

#include "BinaryArchive.h"

namespace Sunset
{
    template <>
    void BinaryInputArchive::Read(std::string& value)
    {
        size_t size = 0;

        Read(size);
        value.resize(size);

        if (size > 0)
            m_Stream.read(value.data(), static_cast<std::streamsize>(size));
    }

    template <>
    void BinaryOutputArchive::Write(std::string& value)
    {
        size_t size = value.size();

        Write(size);

        if (size > 0)
            m_Stream.write(value.data(), static_cast<std::streamsize>(size));
    }
} // Sunset