#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise):
    shapeSettings(shapeSettings),
    noiseFilters(NoiseFilterFactoty::CreateNoiseFilters(noise, shapeSettings)) { }

ShapeGenerator::~ShapeGenerator() { }

glm::vec3 ShapeGenerator::CalculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
    float firstLayerValue = 0.0f;
    float elevation = 0.0f;

    if (noiseFilters.size() > 0)
    {
        firstLayerValue = noiseFilters[0]->Evaluate(pointOnUnitSphere);
        if (shapeSettings.GetEnabled(0))
            elevation += firstLayerValue;
    }

    for (int i = 0; i < noiseFilters.size(); i++) {
        if (shapeSettings.GetEnabled(i))
        {
            float mask = (shapeSettings.GetUseFirstLayerAsMask(i)) ? firstLayerValue : 1.0f;
            elevation += noiseFilters[i]->Evaluate(pointOnUnitSphere) * mask;
        }
    }
    return pointOnUnitSphere * shapeSettings.GetPlanetRadius() * (1.0f + elevation);
}
