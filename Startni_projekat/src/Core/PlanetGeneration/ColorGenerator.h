#ifndef COLOR_GENERATOR_CLASS_H
#define COLOR_GENERATOR_CLASS_H

#include <memory>
#include <glm/glm.hpp>
#include <vector>

#include "Settings/ColorSetting.h"
#include "MinMax.h"
#include "Textures/GradientTexture.h"
#include "shaderClass.h"
#include "Noise/Noise.h"
#include "Noise/NoiseFilters/NoiseFilter.h"
#include "Noise/NoiseFilters/NoiseFilterFactory.h"

class ColorGenerator
{
private:
	ColorSettings settings;
	MinMax elevationMinMax;
	std::unique_ptr<GradientTexture> texture;

	Noise biomeNoise;
	mutable std::unique_ptr<NoiseFilter> biomeNoiseFilter;

public:
	ColorGenerator();

	ColorSettings& GetSettings();

	void UpdateElevation(const MinMax& minMax);
	void UpdateBiomeNoiseFilter();
	void UpdateColors();
	float BiomePercentFromPoint(const glm::vec3& pointOnUnitSphere) const;
	void SetShaderProgramParameters(const Shader& shader) const;
};

#endif