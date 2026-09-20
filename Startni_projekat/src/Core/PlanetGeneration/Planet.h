#ifndef PLANET_H
#define PLANET_H

#include <memory>
#include <glm/glm.hpp>
#include "TerrainFace.h"
#include "Settings/ShapeSettings.h"
#include "ShapeGenerator.h"
#include "Noise/Noise.h"
#include "Noise/NoiseSettings.h"

class Planet {
private:
	ShapeSettings shapeSettings;
	Noise noise;

	unsigned int resolution;
	std::unique_ptr<TerrainFace> faces[6];
	bool renderFace[6];

	void GeneratePlanet();
public:
	static std::unique_ptr<Planet> CreateUniq(float radius, unsigned int resolution);

	Planet(float radius, unsigned int resolution);
	~Planet();

	void Render() const;

	unsigned int GetResolution() const { return resolution; }
	void SetResolution(unsigned int res);

	float GetRadius() const { return shapeSettings.GetPlanetRadius(); }
	void SetRadius(float radius);

	bool& GetRenderFaceRef(int index) { return renderFace[index]; }

	NoiseSettings& GetNoiseSettings(int layerIndex) { return shapeSettings.GetNoiseSettings(layerIndex); }
	int GetNumberOfNoiseLayers() const { return shapeSettings.GetNumberOfLayers(); }
	void AddNewNoiseLayer() { shapeSettings.AddNewNoiseLayer(); }

	bool& GetLayerEnabledReference(int index) { return shapeSettings.GetLayerEnabledRef(index); }
	bool& GetLayerUseFirstLayerAsMask(int index) { return shapeSettings.GetLayerUseFirstLayerAsMaskRef(index); }

	void SetSeedForNoise(int seed);

	void RegeneratePlanet();
};

#endif // !PLANET_H
