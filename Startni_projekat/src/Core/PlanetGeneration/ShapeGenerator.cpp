#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(const ShapeSettings& shapeSettings): shapeSettings(shapeSettings) { }

ShapeGenerator::~ShapeGenerator() { }

glm::vec3 ShapeGenerator::CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
    return pointOnUnitSphere * shapeSettings.GetPlanetRadius();
}
