#include "NoiseFilter.h"

NoiseFilter::NoiseFilter(Noise& noise): noise(noise) { }

NoiseFilter::~NoiseFilter() { }

float NoiseFilter::Evaluate(const glm::vec3& point)
{
    float noiseValue = (noise.Evaluate(point) + 1) * 0.5f;
    return noiseValue;
}
