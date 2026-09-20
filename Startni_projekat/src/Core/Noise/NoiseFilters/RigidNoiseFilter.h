#ifndef RIGID_NOISE_FILTER_H
#define RIGID_NOISE_FILTER_H

#include "NoiseFilter.h"
#include <cmath>
#include <algorithm>

class RigidNoiseFilter : public NoiseFilter
{
private:
	static float clamp(float value);

public:
	RigidNoiseFilter(Noise& noise, NoiseSettings& settings);
	~RigidNoiseFilter();

	float Evaluate(const glm::vec3& point) override;
};

#endif // !RIGID_NOISE_FILTER_H
