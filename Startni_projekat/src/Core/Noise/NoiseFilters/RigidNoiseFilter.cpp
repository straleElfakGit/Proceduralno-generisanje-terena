#include "RigidNoiseFilter.h"

float RigidNoiseFilter::clamp(float value)
{
    return std::max(0.0f, std::min(value, 1.0f));
}

RigidNoiseFilter::RigidNoiseFilter(Noise& noise, NoiseSettings& settings) :
    NoiseFilter(noise, settings) { }

RigidNoiseFilter::~RigidNoiseFilter() { }

float RigidNoiseFilter::Evaluate(const glm::vec3& point)
{
    float noiseValue = 0.0f;
    float frequency = noiseSettings.baseRoughness;
    float amplitude = 1.0f;
    float weight = 1.0f;

    for (int i = 1; i <= noiseSettings.numberOfOctaves; i++) {
        float v = 1.0 - abs(noise.Evaluate(point * frequency + noiseSettings.center));
        v *= v;
        v *= weight;
        weight = RigidNoiseFilter::clamp(v * noiseSettings.weightMultiplier);

        noiseValue += v * amplitude;
        frequency *= noiseSettings.roughness;
        amplitude *= noiseSettings.persistance;
    }

    noiseValue = std::max(0.0f, noiseValue - noiseSettings.minValue);
    return noiseValue * noiseSettings.strength;
}
