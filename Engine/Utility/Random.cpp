//
// Created by sunvy on 15/01/2026.
//

#include "Random.h"

#include <random>

namespace  SunsetEngine
{
    Random::Random(int seed, float frequency)
    : m_Seed(seed)
    , m_Frequency(frequency)
    {
        std::array<int, 256> p;
        std::mt19937 rng(69);

        for (int i = 0; i < 256; i++)
            p[i] = i;

        for (int i = 255; i > 0; --i)
        {
            std::uniform_int_distribution<int> dist(0, i);
            int j = dist(rng);
            std::swap(p[i], p[j]);
        }

        for (int i = 0; i < 512; i++)
            perm[i] = p[i & 255];
    }
}

namespace
{
    template <typename T>
    T Fade(T t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    template <typename T>
    T Lerp(T a, T b, T t)
    {
        return a + (b - a) * t;
    }

    int hash(int x, int y, const int seed)
    {
        int h = x * 374761393
              + y * 668265263
              + seed * 1442695041;

        h = (h ^ (h >> 13)) * 1274126177;
        return h;
    }

    void Gradient(int ix, int iy, float& gx, float& gy, const int seed)
    {
        int h = hash(ix, iy, seed) & 3;

        switch (h)
        {
            case 0: gx =  1; gy =  0; break;
            case 1: gx = -1; gy =  0; break;
            case 2: gx =  0; gy =  1; break;
            case 3: gx =  0; gy = -1; break;
        }
    }

    float DotGradient(int ix, int iy, float x, float y, const int seed)
    {
        float gx, gy;
        Gradient(ix, iy, gx, gy, seed);

        float dx = x - ix;
        float dy = y - iy;

        return dx * gx + dy * gy;
    }

    template<typename T>
    T grad(int hash, T x, T y, T z)
    {
        // 12 directions possibles
        int h = hash & 15;
        T u = h < 8 ? x : y;
        T v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }
}

namespace SunsetEngine
{
    template<typename T>
        PerlinNoise<T>::PerlinNoise(int seed, float frequency)
        : Random(seed, frequency)
    {
    }

    template<typename T>
    float PerlinNoise<T>::operator()(const glm::vec<2, T> &vec)
    {
        return operator()(vec.x, vec.y);
    }

    template<typename T>
    float PerlinNoise<T>::operator()(T x, T y)
    {
        float xf = x * m_Frequency;
        float yf = y * m_Frequency;

        int x0 = static_cast<int>(floor(xf));
        int y0 = static_cast<int>(floor(yf));

        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float u = Fade(xf - x0);
        float v = Fade(yf - y0);

        float n00 = DotGradient(x0, y0, xf, yf, m_Seed);
        float n01 = DotGradient(x0, y1, xf, yf, m_Seed);
        float n10 = DotGradient(x1, y0, xf, yf, m_Seed);
        float n11 = DotGradient(x1, y1, xf, yf, m_Seed);

        float ix0 = Lerp(n00, n10, u);
        float iy0 = Lerp(n01, n11, u);

        return Lerp(ix0, iy0, v);
    }

    template<typename T>
    float PerlinNoise<T>::operator()(const glm::vec<3, T> &vec)
    {
        return operator()(vec.x, vec.y, vec.z);
    }

    template<typename T>
    float PerlinNoise<T>::operator()(T x, T y, T z)
    {
        int X = (int)std::floor(x) & 255;
        int Y = (int)std::floor(y) & 255;
        int Z = (int)std::floor(z) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        T u = Fade(x);
        T v = Fade(y);
        T w = Fade(z);

        int A  = perm[X] + Y;
        int AA = perm[A] + Z;
        int AB = perm[A + 1] + Z;

        int B  = perm[X + 1] + Y;
        int BA = perm[B] + Z;
        int BB = perm[B + 1] + Z;

        return Lerp(
            Lerp(
                Lerp(grad(perm[AA], x, y, z),
                     grad(perm[BA], x - 1, y, z), u),
                Lerp(grad(perm[AB], x, y - 1, z),
                     grad(perm[BB], x - 1, y - 1, z), u),
                v),
            Lerp(
                Lerp(grad(perm[AA + 1], x, y, z - 1),
                     grad(perm[BA + 1], x - 1, y, z - 1), u),
                Lerp(grad(perm[AB + 1], x, y - 1, z - 1),
                     grad(perm[BB + 1], x - 1, y - 1, z - 1), u),
                v),
            w);
    }

    template struct PerlinNoise<float>;
    template struct PerlinNoise<double>;
    template struct PerlinNoise<int>;
}

/**********************************/
/* Simplex Noise                  */
/**********************************/

namespace
{
    const int grad2[8][2] = {
        { 1, 1}, {-1, 1}, { 1,-1}, {-1,-1},
        { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}
    };

    const int grad3[12][3] = {
        {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
        {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
        {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
    };

    template<typename T>
    T dot(const int* g, T x, T y, T z)
    {
        return g[0]*x + g[1]*y + g[2]*z;
    }
}

namespace SunsetEngine
{
    template<typename T>
    SimplexNoise<T>::SimplexNoise(int seed, float frequency)
        : Random(seed, frequency)
    {
    }

    template<typename T>
    float SimplexNoise<T>::operator()(const glm::vec<2, T> &vec)
    {
        return operator()(vec.x, vec.y);
    }

    template<typename T>
    float SimplexNoise<T>::operator()(T x, T y)
    {
        const T xf = x * static_cast<T>(m_Frequency);
        const T yf = y * static_cast<T>(m_Frequency);

        // --- Constantes skew / unskew pour 2D
        const T F2 = (T)0.5 * ((T)std::sqrt((T)3.0) - (T)1.0);
        const T G2 = ((T)3.0 - (T)std::sqrt((T)3.0)) / (T)6.0;

        // --- Skew de l'espace
        T s = (xf + yf) * F2;
        int i = (int)std::floor(xf + s);
        int j = (int)std::floor(yf + s);

        // --- Unskew
        T t = (i + j) * G2;
        T X0 = i - t;
        T Y0 = j - t;

        T x0 = xf - X0;
        T y0 = yf - Y0;

        // --- Déterminer simplex (triangle)
        int i1, j1;

        if (x0 > y0)
        {
            i1 = 1; j1 = 0;
        }
        else
        {
            i1 = 0; j1 = 1;
        }

        // --- Offsets
        T x1 = x0 - i1 + G2;
        T y1 = y0 - j1 + G2;

        T x2 = x0 - 1.0 + 2.0 * G2;
        T y2 = y0 - 1.0 + 2.0 * G2;

        // --- Hash
        int ii = i & 255;
        int jj = j & 255;

        int gi0 = perm[ii + perm[jj]] % 8;
        int gi1 = perm[ii + i1 + perm[jj + j1]] % 8;
        int gi2 = perm[ii + 1 + perm[jj + 1]] % 8;

        // --- Contribution
        auto contrib = [&](T x, T y, int gi)
        {
            T t = 0.5 - x*x - y*y;
            if (t < 0) return (T)0;

            t *= t;
            return t * t * (grad2[gi][0] * x + grad2[gi][1] * y);
        };

        T n0 = contrib(x0, y0, gi0);
        T n1 = contrib(x1, y1, gi1);
        T n2 = contrib(x2, y2, gi2);

        // --- Normalisation (standard simplex 2D)
        return 70.0f * (n0 + n1 + n2);
    }

    template<typename T>
    float SimplexNoise<T>::operator()(const glm::vec<3, T> &vec)
    {
        return operator()(vec.x, vec.y, vec.z);
    }

    template<typename T>
    float SimplexNoise<T>::operator()(T x, T y, T z)
    {
        const T xf = x * static_cast<T>(m_Frequency);
        const T yf = y * static_cast<T>(m_Frequency);
        const T zf = z * static_cast<T>(m_Frequency);

        // --- Constantes skew / unskew
        const T F3 = (T)1.0 / (T)3.0;
        const T G3 = (T)1.0 / (T)6.0;

        // --- Skew de l'espace
        T s = (xf + yf + zf) * F3;
        int i = floor(xf + s);
        int j = floor(yf + s);
        int k = floor(zf + s);

        // --- Unskew
        T t = (i + j + k) * G3;
        T X0 = i - t;
        T Y0 = j - t;
        T Z0 = k - t;

        T x0 = xf - X0;
        T y0 = yf - Y0;
        T z0 = zf - Z0;

        // --- Déterminer le simplex (tetrahedron)
        int i1, j1, k1;
        int i2, j2, k2;

        if (x0 >= y0)
        {
            if (y0 >= z0)      { i1 = 1; j1 = 0; k1 = 0;  i2 = 1; j2 = 1; k2 = 0; }
            else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0;  i2 = 1; j2 = 0; k2 = 1; }
            else               { i1 = 0; j1 = 0; k1 = 1;  i2 = 1; j2 = 0; k2 = 1; }
        }
        else
        {
            if (y0 < z0)       { i1 = 0; j1 = 0; k1 = 1;  i2 = 0; j2 = 1; k2 = 1; }
            else if (x0 < z0)  { i1 = 0; j1 = 1; k1 = 0;  i2 = 0; j2 = 1; k2 = 1; }
            else               { i1 = 0; j1 = 1; k1 = 0;  i2 = 1; j2 = 1; k2 = 0; }
        }

        // --- Offsets des coins
        T x1 = x0 - i1 + G3;
        T y1 = y0 - j1 + G3;
        T z1 = z0 - k1 + G3;

        T x2 = x0 - i2 + 2.0 * G3;
        T y2 = y0 - j2 + 2.0 * G3;
        T z2 = z0 - k2 + 2.0 * G3;

        T x3 = x0 - 1.0 + 3.0 * G3;
        T y3 = y0 - 1.0 + 3.0 * G3;
        T z3 = z0 - 1.0 + 3.0 * G3;

        // --- Hash gradients
        int ii = i & 255;
        int jj = j & 255;
        int kk = k & 255;

        int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
        int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
        int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
        int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

        // --- Contribution des 4 coins
        T n0, n1, n2, n3;

        auto contrib = [&](T x, T y, T z, int gi)
        {
            T t = 0.6 - x*x - y*y - z*z;
            if (t < 0) return (T)0.0;
            t *= t;
            return t * t * dot(grad3[gi], x, y, z);
        };

        n0 = contrib(x0, y0, z0, gi0);
        n1 = contrib(x1, y1, z1, gi1);
        n2 = contrib(x2, y2, z2, gi2);
        n3 = contrib(x3, y3, z3, gi3);

        // --- Normalisation
        return 32.0f * (n0 + n1 + n2 + n3);
    }

    template struct SimplexNoise<float>;
    template struct SimplexNoise<double>;
    template struct SimplexNoise<int>;
}
