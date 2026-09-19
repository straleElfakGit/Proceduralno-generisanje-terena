#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "Noise/NoiseFilter.h"
#include "Noise/NoiseSettings.h"
#include "Settings/ShapeSettings.h"
#include <glm/glm.hpp>

class ShapeGenerator
{
private:
	ShapeSettings& shapeSettings;
	mutable NoiseFilter noiseFilter;

public:
	ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise);
	~ShapeGenerator();

	glm::vec3 CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;
};

#endif // !SHAPE_GENERATOR_H

