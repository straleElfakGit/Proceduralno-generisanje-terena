#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise):
    shapeSettings(shapeSettings),
    noiseFilter(noise) { }

ShapeGenerator::~ShapeGenerator() { }

glm::vec3 ShapeGenerator::CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
    float elevation = noiseFilter.Evaluate(pointOnUnitSphere);
    return pointOnUnitSphere * shapeSettings.GetPlanetRadius() * (1.0f + elevation);
}
