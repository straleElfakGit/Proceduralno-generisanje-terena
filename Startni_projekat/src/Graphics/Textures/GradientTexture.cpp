#include "GradientTexture.h"

GradientTexture::GradientTexture(int resolution, GLuint slot)
	:unit(slot), resolution(resolution)
{
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_1D, ID);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, resolution, 0, GL_RGB, GL_FLOAT, nullptr);

	glBindTexture(GL_TEXTURE_1D, 0);
}

GradientTexture::~GradientTexture()
{
	Delete();
}

void GradientTexture::SetColors(const std::vector<glm::vec3>& colors)
{
	if (static_cast<int>(colors.size()) != resolution)
	{
		LOG_ERR("GradientTexture: expected {} colors, got {}", resolution, colors.size());
		return;
	}
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_1D, ID);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, resolution, GL_RGB, GL_FLOAT, colors.data());
	glBindTexture(GL_TEXTURE_1D, 0);
}

void GradientTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_1D, ID);
}

void GradientTexture::Unbind() const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_1D, 0);
}

void GradientTexture::DeleteSpecific()
{
	glDeleteTextures(1, &ID);
}
