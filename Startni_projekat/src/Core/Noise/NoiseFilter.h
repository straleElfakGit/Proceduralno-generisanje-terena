#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include <glm/glm.hpp>
#include "Noise/Noise.h"

class NoiseFilter
{
private:
	Noise& noise;

public:
	NoiseFilter(Noise& noise);
	~NoiseFilter();

	float Evaluate(const glm::vec3& point);
};

#endif // !NOISE_FILTER_H
