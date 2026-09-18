#ifndef PLANET_H
#define PLANET_H

#include <memory>
#include "TerrainFace.h"
#include "Settings/ShapeSettings.h"
#include "ShapeGenerator.h"
#include "Noise/Noise.h"

class Planet {
private:
	ShapeSettings shapeSettings;
	Noise noise;

	unsigned int resolution;
	std::unique_ptr<TerrainFace> faces[6];

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

	void SetSeedForNoise(int seed);
};

#endif // !PLANET_H
