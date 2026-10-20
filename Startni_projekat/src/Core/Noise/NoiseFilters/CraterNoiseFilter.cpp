#include "CraterNoiseFilter.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <glm/gtc/constants.hpp>

namespace
{
	float Uniform01(std::mt19937& rng)
	{
		return (rng() >> 8) * (1.0f / 16777216.0f);
	}
}

CraterFilter::CraterFilter(Noise& noise, NoiseSettings& noiseSettings) :
	NoiseFilter(noise, noiseSettings),
	settings(noiseSettings.craters),
	strength(noiseSettings.strength)
{
	GenerateCraters();
}

CraterFilter::~CraterFilter() {}

float CraterFilter::SmoothMin(float a, float b, float k)
{
	k = std::max(k, 1e-5f);
	const float h = glm::clamp((b - a + k) / (2.0f * k), 0.0f, 1.0f);
	return a * h + b * (1.0f - h) - k * h * (1.0f - h);
}

float CraterFilter::SmoothMax(float a, float b, float k)
{
	k = std::max(k, 1e-5f);
	const float h = glm::clamp((a - b + k) / (2.0f * k), 0.0f, 1.0f);
	return a * h + b * (1.0f - h) + k * h * (1.0f - h);
}

float CraterFilter::CraterShape(float x) const
{
	const float cavity = x * x - 1.0f;

	const float rimX = std::min(x - 1.0f - settings.rimWidth, 0.0f);
	const float rim = settings.rimSteepness * rimX * rimX;

	float shape = SmoothMax(cavity, settings.floorHeight, settings.smoothness);
	shape = SmoothMin(shape, rim, settings.smoothness);
	return shape;
}

void CraterFilter::GenerateCraters()
{
	std::mt19937 rng(static_cast<unsigned int>(settings.seed));

	const int count = std::max(settings.numberOfCraters, 0);
	craters.clear();
	craters.reserve(count);

	const float xMax = std::max(1.0f + std::max(settings.rimWidth, 0.0f),
		std::sqrt(1.0f + std::max(settings.smoothness, 0.0f)));

	for (int i = 0; i < count; i++)
	{
		const float z = 2.0f * Uniform01(rng) - 1.0f;
		const float phi = 2.0f * glm::pi<float>() * Uniform01(rng);
		const float sizeRandom = Uniform01(rng);

		const float r = std::sqrt(std::max(0.0f, 1.0f - z * z));

		Crater c;
		c.center = glm::vec3(r * std::cos(phi), r * std::sin(phi), z);

		const float t = std::pow(sizeRandom, std::max(settings.sizeSkew, 0.01f));
		c.radius = glm::mix(settings.minRadius, settings.maxRadius, t);
		c.radius = std::max(c.radius, 1e-4f);

		const float influence = xMax * c.radius;
		c.influence2 = influence * influence;

		craters.push_back(c);
	}
}

float CraterFilter::Evaluate(const glm::vec3& point)
{
	float height = 0.0f;

	for (const Crater& c : craters)
	{
		const glm::vec3 d = point - c.center;
		const float d2 = glm::dot(d, d);

		if (d2 > c.influence2)
			continue;

		const float x = std::sqrt(d2) / c.radius;
		height += CraterShape(x) * c.radius;
	}

	return height * strength;
}
