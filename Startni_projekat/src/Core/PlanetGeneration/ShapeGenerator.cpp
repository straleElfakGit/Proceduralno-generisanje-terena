#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(ShapeSettings& shapeSettings, Noise& noise):
    shapeSettings(shapeSettings),
    noiseFilters(NoiseFilterFactoty::CreateNoiseFilters(noise, shapeSettings)),
    elevationMinMax() { }

ShapeGenerator::~ShapeGenerator() { }

float ShapeGenerator::CalculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const
{
    float firstLayerValue = 0.0f;
    float elevation = 0.0f;

    if (noiseFilters.size() > 0)
    {
        firstLayerValue = noiseFilters[0]->Evaluate(pointOnUnitSphere);
        if (shapeSettings.GetEnabled(0))
            elevation += firstLayerValue;
    }

    for (int i = 1; i < noiseFilters.size(); i++) {
        if (shapeSettings.GetEnabled(i))
        {
            float mask = (shapeSettings.GetUseFirstLayerAsMask(i)) ? firstLayerValue : 1.0f;
            elevation += noiseFilters[i]->Evaluate(pointOnUnitSphere) * mask;
        }
    }
    elevationMinMax.AddValue(elevation);
    return elevation;
}

float ShapeGenerator::CalculateScaledElevation(float unscaledElevation) const
{
    float elevation = std::max(0.0f, unscaledElevation);
    elevation = shapeSettings.GetPlanetRadius() * (1.0f + elevation);
    return elevation;
}