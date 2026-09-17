#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "Settings/ShapeSettings.h"
#include <glm/glm.hpp>

class ShapeGenerator
{
private:
	ShapeSettings shapeSettings;

public:
	ShapeGenerator(const ShapeSettings& shapeSettings);
	~ShapeGenerator();

	glm::vec3 CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;
};

#endif // !SHAPE_GENERATOR_H

