#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include <glm/glm.hpp>
#include "Noise/Noise.h"
#include "Noise/NoiseSettings.h"

class NoiseFilter
{
private:
	Noise& noise;
	NoiseSettings& noiseSettings;

public:
	NoiseFilter(Noise& noise, NoiseSettings& settings);
	~NoiseFilter();

	float Evaluate(const glm::vec3& point);
};

#endif // !NOISE_FILTER_H
