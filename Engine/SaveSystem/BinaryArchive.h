//
// Created by sunvy on 04/07/2026.
//

#pragma once

namespace Sunset
{
    template <typename>
    inline constexpr bool always_false_v = false;

    template <typename Archive, typename T>
    void Serialize(Archive& ar, T& value)
    {
        static_assert(always_false_v<T>, "Serialize<T> must be implemented for this type.");
    }

    class BinaryInputArchive
    {
    public:
        explicit BinaryInputArchive(std::ifstream& stream)
            : m_Stream(stream)
        {
        }

        template <typename T>
        void operator()(T& value)
        {
            BinaryInputArchive::Read(value);
        }

    private:
        template <typename T>
        void Read(T& value)
        {
            if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
            {
                m_Stream.read(reinterpret_cast<char*>(&value), sizeof(T));
            }
            else
            {
                Sunset::Serialize(*this, value);
            }
        }

        template <typename T>
        void Read(std::vector<T>& values)
        {
            size_t size = 0;

            Read(size);

            values.resize(size);

            for (auto& value : values)
            {
                BinaryInputArchive::Read(value);
            }
        }
    private:
        std::ifstream& m_Stream;
    };

    template <>
    void BinaryInputArchive::Read<std::string>(std::string& value);

    class BinaryOutputArchive
    {
    public:
        explicit BinaryOutputArchive(std::ostream& stream)
            : m_Stream(stream)
        {
        }

        template <typename T>
        void operator()(T& value)
        {
            Write(value);
        }

    private:
        std::ostream& m_Stream;

    private:
        template <typename T>
        void Write(T& value)
        {
            if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
            {
                m_Stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
            }
            else
            {
                Sunset::Serialize(*this, value);
            }
        }

        template <typename T>
        void Write(std::vector<T>& values)
        {
            size_t size = values.size();

            Write(size);

            for (auto& value : values)
            {
                Write(value);
            }
        }
    };

    template <>
    void BinaryOutputArchive::Write<std::string>(std::string& value);
} // Sunset