#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <array>
#include <algorithm>
#include <random>
#include <numeric>

#include <glm/glm.hpp>

template <typename T>
class PerlinNoise
{
private:
    static const unsigned long long PERMUTATION_COUNT = 256;

    std::array<unsigned int, PERMUTATION_COUNT * 2> p;
    int seed;

    T Fade(T t) const
    {
        return t * t * t * (t * (t * (T)6 - (T)15) + (T)10);
    }

    T Lerp(T t, T a, T b) const
    {
        return a + t * (b - a);
    }

    T Grad(int hash, T x, T y, T z) const
    {
        int h = hash & 15;
        T u = h < 8 ? x : y;
        T v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    PerlinNoise(int seed = std::random_device{}()) : seed(seed)
    {
        GeneratePermutations();
    }

    int GetSeed() const { return seed; }
    void SetSeed(int seed) { seed = seed; }

    void GeneratePermutations()
    {
        std::iota(p.begin(), p.begin() + PerlinNoise::PERMUTATION_COUNT, 0);

        std::default_random_engine rndEngine(seed);
        std::shuffle(p.begin(), p.begin() + PerlinNoise::PERMUTATION_COUNT, rndEngine);

        for (unsigned int i = 0; i < PerlinNoise::PERMUTATION_COUNT; ++i)
            p[PerlinNoise::PERMUTATION_COUNT + i] = p[i];
    }

    T Noise(T x, T y, T z) const
    {
        int X = (int)glm::floor(x) & 255;
        int Y = (int)glm::floor(y) & 255;
        int Z = (int)glm::floor(z) & 255;
        x -= glm::floor(x);
        y -= glm::floor(y);
        z -= glm::floor(z);
        T u = Fade(x);
        T v = Fade(y);
        T w = Fade(z);

        unsigned int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        unsigned int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
            Grad(p[BA], x - 1, y, z)),
            Lerp(u, Grad(p[AB], x, y - 1, z),
                Grad(p[BB], x - 1, y - 1, z))),
            Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
                Grad(p[BA + 1], x - 1, y, z - 1)),
                Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                    Grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }
};

#endif // !PERLIN_NOISE_H