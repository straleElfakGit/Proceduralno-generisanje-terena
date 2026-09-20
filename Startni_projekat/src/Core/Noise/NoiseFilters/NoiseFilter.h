#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include "Noise/Noise.h"
#include "Noise/NoiseSettings.h"

class NoiseFilter
{
protected:
	Noise& noise;
	NoiseSettings& noiseSettings;

public:
	NoiseFilter(Noise& noise, NoiseSettings& settings) : noise(noise), noiseSettings(settings) {}
	virtual ~NoiseFilter() {};

	virtual float Evaluate(const glm::vec3& point) = 0;
};

#endif // !NOISE_FILTER_H
