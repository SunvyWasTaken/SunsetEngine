//
// Created by sunvy on 04/07/2026.
//

#pragma once

namespace Sunset
{
    template <typename Archive, typename T>
    void Serialize(Archive& ar, T& value)
    {
        static_assert(always_false_v<T>, "Serialize<T> must be implemented for this type.");
    }

    template <typename Archive, typename T>
    void Serialize(Archive& ar, glm::vec<2, T, glm::defaultp>& value)
    {
        ar(value.x);
        ar(value.y);
    }

    template <typename Archive, typename T>
    void Serialize(Archive& ar, glm::vec<3, T, glm::defaultp>& value)
    {
        ar(value.x);
        ar(value.y);
        ar(value.z);
    }

    template <typename Archive, typename T>
    void Serialize(Archive& ar, glm::vec<4, T, glm::defaultp>& value)
    {
        ar(value.x);
        ar(value.y);
        ar(value.z);
        ar(value.w);
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
                Serialize(*this, value);
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

        template <typename T, std::size_t N>
        void Read(std::array<T, N>& values)
        {
            for (auto& value : values)
            {
                Read(value);
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
                Serialize(*this, value);
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

        template <typename T, std::size_t N>
        void Write(std::array<T, N>& values)
        {
            for (auto& value : values)
            {
                Write(value);
            }
        }
    };

    template <>
    void BinaryOutputArchive::Write<std::string>(std::string& value);
} // Sunset