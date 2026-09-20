#include "ColorGenerator.h"

namespace
{
	const int kTextureResolution = 50;
	const GLuint kTextureSlot = 1;
}

ColorGenerator::ColorGenerator()
{
	texture = std::make_unique<GradientTexture>(kTextureResolution, kTextureSlot);
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

void ColorGenerator::UpdateColors()
{
	const Gradient sorted = settings.gradient.Sorted();

	std::vector<glm::vec3> colors(kTextureResolution);
	for (int i = 0; i < kTextureResolution; i++)
		colors[i] = sorted.Evaluate(i / (kTextureResolution - 1.0f));

	texture->SetColors(colors);
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