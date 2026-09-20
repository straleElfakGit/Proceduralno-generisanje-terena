#include "SimpleNoiseFilter.h"
#include <algorithm>

SimpleNoiseFilter::SimpleNoiseFilter(Noise& noise, NoiseSettings& settings): NoiseFilter(noise, settings) { }

SimpleNoiseFilter::~SimpleNoiseFilter() { }

float SimpleNoiseFilter::Evaluate(const glm::vec3& point)
{
    float noiseValue = 0.0f;
    float frequency = noiseSettings.baseRoughness;
    float amplitude = 1.0f;

    for (int i = 1; i <= noiseSettings.numberOfOctaves; i++) {
        float v = noise.Evaluate(point * frequency + noiseSettings.center);
        noiseValue += (v + 1.0f) * 0.5f * amplitude;
        frequency *= noiseSettings.roughness;
        amplitude *= noiseSettings.persistance;
    }

    noiseValue = std::max(0.0f, noiseValue - noiseSettings.minValue);
    return noiseValue * noiseSettings.strength;
}
