#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "Noise/NoiseFilters/NoiseFilter.h"
#include "Noise/NoiseSettings.h"
#include "Settings/ShapeSettings.h"
#include "Noise/NoiseFilters/NoiseFilterFactory.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class ShapeGenerator
{
private:
	ShapeSettings& shapeSettings;
	mutable std::vector<std::unique_ptr<NoiseFilter>> noiseFilters;

public:
	ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise);
	~ShapeGenerator();

	glm::vec3 CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;
};

#endif // !SHAPE_GENERATOR_H

