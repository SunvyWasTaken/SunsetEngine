//
// Created by sunvy on 15/01/2026.
//

#ifndef SUNSETCRAFT_RANDOM_H
#define SUNSETCRAFT_RANDOM_H

namespace SunsetEngine
{
    struct Random
    {
        Random(int seed, float frequency);

        int m_Seed;
        float m_Frequency;

        std::array<int, 512> perm;
    };

    template <typename T>
    struct PerlinNoise : public Random
    {
        PerlinNoise(int seed = 0, float frequency = 0);

        float operator()(const glm::vec<2, T>& vec);

        float operator()(T x, T y);

        float operator()(const glm::vec<3, T>& vec);

        float operator()(T x, T y, T z);
    };

    template <typename T>
    struct SimplexNoise : public Random
    {
        SimplexNoise(int seed = 0, float frequency = 0);

        float operator()(const glm::vec<2, T>& vec);

        float operator()(T x, T y);

        float operator()(const glm::vec<3, T>& vec);

        float operator()(T x, T y, T z);
    };

    template <typename T, int Nbr, float freq = 1.f>
    requires std::is_base_of_v<Random, T>
    struct Fbm
    {
        std::array<T, Nbr> Noises;

        explicit Fbm(int seed = 0)
        {
            constexpr float frequency = freq / (Nbr + 1);
            for (int i = 0; i < Nbr; ++i)
            {
                Noises[i] = T(seed, frequency * i);
            }
        }

        template <typename type>
        float operator()(const type& vec)
        {
            if constexpr (requires { vec.x; vec.y; vec.z; })
                return operator()(vec.x, vec.y, vec.z);
            else if constexpr (requires { vec.x; vec.y; })
                return operator()(vec.x, vec.y);
            else
                static_assert([]{return false;}(), "Unsupported vector type");
        }


        template <typename ...Type>
        float operator()(Type&& ...type)
        {
            float n = 0;
            for (int i = 0; i < Nbr; ++i)
                n += Noises[i](std::forward<Type>(type)...);

            return n;
        }
    };

}

#endif //SUNSETCRAFT_RANDOM_H