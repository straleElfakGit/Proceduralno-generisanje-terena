#ifndef GRADIENT_TEXTURE_H
#define GRADIENT_TEXTURE_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Logging/Logger.h"
#include "GLResource.h"

class GradientTexture : public GLResource
{
private:
	GLuint unit;
	int resolution;

protected:
	virtual void DeleteSpecific() override;

public:
	GradientTexture(int resolution, GLuint slot);
	~GradientTexture();

	void SetColors(const std::vector<glm::vec3>& cloros);

	virtual void Bind() const override;
	virtual void Unbind() const override;
};


#endif //!GRADIENT_TEXTURE_H
