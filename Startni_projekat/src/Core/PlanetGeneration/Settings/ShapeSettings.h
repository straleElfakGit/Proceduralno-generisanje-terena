#ifndef SHAPE_SETTINGS_H
#define SHAPE_SETTINGS_H

#include "Noise/NoiseSettings.h"

class ShapeSettings
{
private:
	float planetRadius = 1.0f;
	NoiseSettings noiseSettings;

public:
	ShapeSettings(float planetRadius): planetRadius(planetRadius), noiseSettings() {}
	~ShapeSettings() {}

	float GetPlanetRadius() const { return planetRadius; }
	void SetPlanetRadius(float planetR) { this->planetRadius = planetR; }

	NoiseSettings& GetNoiseSettings() { return noiseSettings; }
	const NoiseSettings& GetNoiseSettingsConst() const { return noiseSettings; }
};

#endif // #ifndef SHAPE_SETTINGS_H
