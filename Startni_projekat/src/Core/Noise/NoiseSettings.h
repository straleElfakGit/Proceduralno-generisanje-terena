#ifndef NOISE_SETTINGS_H
#define NOISE_SETTINGS_H

#include<glm/glm.hpp>

#include "Logging/Logger.h"
#include "NoiseFilters/NoiseFilterType.h"
#include "CraterSettings.h"

struct NoiseSettings
{
	FilterType filterType;
	
	float strength;
	float roughness;
	float baseRoughness;
	float persistance;
	glm::vec3 center;
	int numberOfOctaves;
	float minValue;

	float weightMultiplier;

	CraterSettings craters;

	NoiseSettings(float strength = 1.0f, float roughness = 2.0f, const glm::vec3& center = glm::vec3(0.0f), 
		int numberOfOcaves = 1, float baseRoughness = 1.0f, float persistance = 0.5f, float minValue = 1.0f) :
		strength(strength),
		roughness(roughness),
		center(center),
		numberOfOctaves(numberOfOcaves),
		baseRoughness(baseRoughness),
		persistance(persistance),
		minValue(minValue),
		filterType(FilterType::Simple),
		weightMultiplier(1.0f),
		craters()
	{
		LOG_FUNC();
	}

	~NoiseSettings() { }
};

#endif // !NOISE_SETTINGS_H

