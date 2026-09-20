#ifndef SIMPLE_NOISE_FILTER_H
#define SIMPLE_NOISE_FILTER_H

#include <glm/glm.hpp>
#include "NoiseFilter.h"

class SimpleNoiseFilter : public NoiseFilter
{
public:
	SimpleNoiseFilter(Noise& noise, NoiseSettings& settings);
	~SimpleNoiseFilter();

	float Evaluate(const glm::vec3& point) override;
};

#endif // !SIMPLE_NOISE_FILTER_H
