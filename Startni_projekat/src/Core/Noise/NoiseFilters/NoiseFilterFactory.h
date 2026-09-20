#ifndef NOISE_FILTER_FACTORY_H
#define NOISE_FILTER_FACTORY_H

#include <memory>
#include <vector>
#include "NoiseFilter.h"
#include "SimpleNoiseFilter.h"
#include "RigidNoiseFilter.h"
#include "Noise/Noise.h"
#include "PlanetGeneration/Settings/ShapeSettings.h"

class NoiseFilterFactoty
{
public:

	static std::vector<std::unique_ptr<NoiseFilter>> CreateNoiseFilters(
		Noise& noise, ShapeSettings& shapeSettings);
};

#endif // !NOISE_FILTER_FACTORY_H
