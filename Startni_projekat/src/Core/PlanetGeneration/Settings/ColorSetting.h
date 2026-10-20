#ifndef COLOR_SETTINGS_H
#define COLOR_SETTINGS_H

#include <algorithm>
#include <vector>
#include <glm/glm.hpp>

#include "Noise/NoiseSettings.h"

struct GradientKey
{
	float position;
	glm::vec3 color;
};

struct Gradient
{
	std::vector<GradientKey> keys;

	Gradient Sorted() const
	{
		Gradient copy = *this;
		std::sort(copy.keys.begin(), copy.keys.end(),
			[](const GradientKey& a, const GradientKey& b) { return a.position < b.position; });
		return copy;
	}

	glm::vec3 Evaluate(float t) const
	{
		if (keys.empty())
			return glm::vec3(0.0f);

		if (t <= keys.front().position)
			return keys.front().color;

		for (size_t i = 1; i < keys.size(); i++)
		{
			if (t <= keys[i].position)
			{
				const GradientKey& a = keys[i - 1];
				const GradientKey& b = keys[i];
				const float span = b.position - a.position;
				const float k = span > 0.0f ? (t - a.position) / span : 1.0f;
				return glm::mix(a.color, b.color, k);
			}
		}

		return keys.back().color;
	}

	glm::vec3 EvaluateFlat(float t) const
	{
		if (keys.empty())
			return glm::vec3(0.0f);

		if (t <= keys.front().position)
			return keys.front().color;

		for (size_t i = 1; i < keys.size(); i++)
		{
			if (t <= keys[i].position)
				return keys[i].color;
		}

		return keys.back().color;
	}
};

struct Biome
{
	Gradient gradient;
	glm::vec3 tint = glm::vec3(1.0f);
	float startHeight = 0.0f;
	float tintPercent = 0.0f;
};

struct BiomeColorSettings
{
	std::vector<Biome> biomes;
	NoiseSettings noise;
	float noiseOffset = 0.0f;
	float noiseStrength = 0.0f;
	float blendAmount = 0.2f;

	BiomeColorSettings()
	{
		Biome b;
		b.gradient.keys = {
			{ 0.0f, glm::vec3(0.85098f,  0.80000f,  0.54902f) },
			{ 0.15f, glm::vec3(0.34510f,  0.92549f,  0.34510f) },
			{ 0.206f, glm::vec3(0.20000f,  0.54902f,  0.20000f) },
			{ 0.411f, glm::vec3(0.45098f,  0.34902f,  0.25098f) },
			{ 0.487f, glm::vec3(0.18039f,  0.16471f,  0.14118f) },
			{ 0.595f, glm::vec3(0.51373f,  0.50980f,  0.50980f) },
			{ 0.85f, glm::vec3(0.82353f,  0.82353f,  0.82353f) },
			{ 1.0f, glm::vec3(1.0f, 1.0f, 1.0f) },
		};
		biomes.push_back(b);

		noise.filterType = FilterType::Simple;
		noise.numberOfOctaves = 2;
		noise.baseRoughness = 1.5f;
		noise.strength = 0.1f;
		noise.persistance = 1.0f;
		noise.minValue = 0.0f;
	}
};

struct ColorSettings 
{
	Gradient oceanColor;
	BiomeColorSettings biomeSettings;

	ColorSettings()
	{
		oceanColor.keys = {
			{ 0.00f, glm::vec3(0.00500f, 0.01500f, 0.15000f) },
			{ 0.35f, glm::vec3(0.02000f, 0.08000f, 0.40000f) },
			{ 0.75f, glm::vec3(0.00000f, 0.25000f, 0.60000f) },
			{ 1.00f, glm::vec3(0.00000f, 0.45000f, 0.55000f) },
		};
	}
};

#endif // #ifndef COLOR_SETTINGS_H
