#ifndef SHAPE_SETTINGS_H
#define SHAPE_SETTINGS_H

#include <vector>
#include "Noise/NoiseSettings.h"
#include "Noise/NoiseLayer.h"

class ShapeSettings
{
private:
	float planetRadius = 1.0f;
	std::vector<NoiseLayer> layers;

public:
	ShapeSettings(float planetRadius, int numberOfLayers): planetRadius(planetRadius), 
		layers(std::vector<NoiseLayer>(numberOfLayers)) {}
	~ShapeSettings() {}

	float GetPlanetRadius() const { return planetRadius; }
	void SetPlanetRadius(float planetR) { this->planetRadius = planetR; }

	NoiseSettings& GetNoiseSettings(int layerIndex) { return layers[layerIndex].GetNoiseSettings(); }
	const NoiseSettings& GetNoiseSettingsConst(int layerIndex) const { return layers[layerIndex].GetNoiseSettingsConst(); }

	int GetNumberOfLayers() const { return layers.size(); }

	bool GetEnabled(int layerIndex) const { return layers[layerIndex].GetEnabled(); }
	bool& GetLayerEnabledRef(int index) { return layers[index].GetEnabledRef(); }

	bool GetUseFirstLayerAsMask(int layerIndex) const { return layers[layerIndex].GetUseFirstLayerAsMask(); }
	bool& GetLayerUseFirstLayerAsMaskRef(int layerIndex) { return layers[layerIndex].GetUseFirstLayerAsMaskRef(); }

	void AddNewNoiseLayer() { layers.push_back(NoiseLayer()); }
};

#endif // #ifndef SHAPE_SETTINGS_H
