#ifndef FRACTAL_NOISE_H
#define FRACTAL_NOISE_H

#include "PerlinNoise.h"

template <typename T>
class FractalNoise
{
public:
    PerlinNoise<T> perlinNoise;

    uint32_t octaveCount;
    T scale;
    T offset;
    T gain;
    T lacunarity;

    FractalNoise(
        const PerlinNoise<T>& perlinNoise,
        T scale = (T)1, T offset = (T)1, uint32_t octaves = 5, T gain = (T)0.5, T lacunarity = (T)2)
        : perlinNoise(perlinNoise), scale(scale), offset(offset), octaveCount(octaves), gain(gain), lacunarity(lacunarity) { }

    T Noise(T x, T y, T z)
    {
        T sum = (T)0;
        T maxAmplitude = (T)0;
        T frequency = (T)1;
        T amplitude = (T)1;

        for (uint32_t i = 0; i < octaveCount; ++i)
        {
            T sampleX = (x / scale + offset) * frequency;
            T sampleY = (y / scale + offset) * frequency;
            T sampleZ = (z / scale + offset) * frequency;

            T noiseVal = (perlinNoise.Noise(sampleX, sampleY, sampleZ) + (T)1.0) * (T)0.5;

            sum += noiseVal * amplitude;
            maxAmplitude += amplitude;

            amplitude *= gain;
            frequency *= lacunarity;
        }

        return sum / maxAmplitude;
    }
};

#endif