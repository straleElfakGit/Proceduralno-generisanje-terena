#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "Noise/NoiseFilters/NoiseFilter.h"
#include "Noise/NoiseSettings.h"
#include "Settings/ShapeSettings.h"
#include "Noise/NoiseFilters/NoiseFilterFactory.h"
#include "MinMax.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>


class ShapeGenerator
{
private:
	ShapeSettings& shapeSettings;
	mutable std::vector<std::unique_ptr<NoiseFilter>> noiseFilters;
	mutable MinMax elevationMinMax;

public:
	ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise);
	~ShapeGenerator();

	glm::vec3 CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;
	const MinMax& GetMinMax() const { return elevationMinMax; }
};

#endif // !SHAPE_GENERATOR_H

