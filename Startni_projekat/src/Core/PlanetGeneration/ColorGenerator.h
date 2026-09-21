#ifndef COLOR_GENERATOR_CLASS_H
#define COLOR_GENERATOR_CLASS_H

#include <memory>
#include <glm/glm.hpp>
#include <vector>

#include "Settings/ColorSetting.h"
#include "MinMax.h"
#include "Textures/GradientTexture.h"
#include "shaderClass.h"

class ColorGenerator
{
private:
	ColorSettings settings;
	MinMax elevationMinMax;
	std::unique_ptr<GradientTexture> texture;

	void UpdateColorsFlat(const Gradient& sorted);
	void UpdateColorsSmooth(const Gradient& sorted);
public:
	ColorGenerator();

	ColorSettings& GetSettings();

	void UpdateElevation(const MinMax& minMax);
	void UpdateColors();
	void SetShaderProgramParameters(const Shader& shader) const;
};

#endif