#include "ColorGenerator.h"

namespace
{
	const int kTextureResolution = 100;
	const GLuint kTextureSlot = 1;
}

ColorGenerator::ColorGenerator()
{
	texture = std::make_unique<GradientTexture>(kTextureResolution * 2, kTextureSlot);
	UpdateBiomeNoiseFilter();
	UpdateColors();
}

ColorSettings& ColorGenerator::GetSettings()
{
	return settings;
}

void ColorGenerator::UpdateElevation(const MinMax& minMax)
{
	elevationMinMax = minMax;
}

void ColorGenerator::UpdateBiomeNoiseFilter()
{
	NoiseSettings& ns = settings.biomeSettings.noise;
	biomeNoiseFilter = NoiseFilterFactoty::CreateNoiseFilter(biomeNoise, ns);
}

float ColorGenerator::BiomePercentFromPoint(const glm::vec3& pointOnUnitSphere) const
{
	float heightPercent = (pointOnUnitSphere.y + 1.0f) / 2.0f;
	heightPercent += (biomeNoiseFilter->Evaluate(pointOnUnitSphere) - settings.biomeSettings.noiseOffset)
		* settings.biomeSettings.noiseStrength;

	float biomeIndex = 0.0f;
	const int numBiomes = static_cast<int>(settings.biomeSettings.biomes.size());
	const float blendRange = settings.biomeSettings.blendAmount / 2.0f + 0.001f;

	for (int i = 0; i < numBiomes; i++)
	{
		const float dst = heightPercent - settings.biomeSettings.biomes[i].startHeight;
		const float weight = glm::clamp((dst + blendRange) / (2.0f * blendRange), 0.0f, 1.0f);
		biomeIndex = biomeIndex * (1.0f - weight) + static_cast<float>(i) * weight;
	}

	return biomeIndex / static_cast<float>(std::max(1, numBiomes - 1));
}


void ColorGenerator::UpdateColors()
{
	const auto& biomes = settings.biomeSettings.biomes;
	const int numBiomes = static_cast<int>(biomes.size());
	if (numBiomes == 0)
		return;

	std::vector<glm::vec3> colors(static_cast<size_t>(kTextureResolution * 2) * numBiomes);

	int index = 0;
	const Gradient sortedOcean = settings.oceanColor.Sorted();
	for (const Biome& biome : biomes)
	{
		const Gradient sorted = biome.gradient.Sorted();
		for (int i = 0; i < kTextureResolution * 2; i++)
		{
			glm::vec3 gradientColor;
			if (i < kTextureResolution)
				gradientColor = sortedOcean.Evaluate(i / (kTextureResolution - 1.0f));
			else
				gradientColor = sorted.Evaluate((i - kTextureResolution) / (kTextureResolution - 1.0f));
			colors[index++] = glm::mix(gradientColor, biome.tint, biome.tintPercent);
		}
	}

	texture->SetColors(kTextureResolution, numBiomes, colors);
}

void ColorGenerator::SetShaderProgramParameters(const Shader& shader) const
{
	const glm::vec2 range = elevationMinMax.IsEmpty()
		? glm::vec2(0.0f, 1.0f)
		: glm::vec2(elevationMinMax.Min(), elevationMinMax.Max());
	shader.setVec2("elevationMinMax", range);
	shader.setInt("colorGradient", kTextureSlot);
	texture->Bind();
}