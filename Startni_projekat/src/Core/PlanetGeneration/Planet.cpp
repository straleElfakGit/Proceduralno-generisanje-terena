#include "Planet.h"

std::unique_ptr<Planet> Planet::CreateUniq(float radius, unsigned int resolution)
{
	return std::make_unique<Planet>(radius, resolution);
}

Planet::Planet(float radius, unsigned int resolution) :
	shapeSettings(radius, 1), resolution(resolution), noise()
{
	for (int i = 0; i < 6; i++)
		renderFace[i] = true;
	GeneratePlanet();
}

Planet::~Planet()
{
}

void Planet::GeneratePlanet()
{
	glm::vec3 directions[6] = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
	};

	ShapeGenerator generator(this->shapeSettings, this->noise);

	for (int i = 0; i < 6; i++) {
		faces[i] = std::make_unique<TerrainFace>(this->resolution, directions[i], generator);
	}
}

void Planet::SetResolution(unsigned int res)
{
	if (this->resolution != res) {
		this->resolution = res;
		ShapeGenerator generator(this->shapeSettings, this->noise);
		for (int i = 0; i < 6; i++)
			if (faces[i] != nullptr)
				faces[i]->SetResolution(res, generator);
	}
}

void Planet::SetRadius(float radius)
{
	if (shapeSettings.GetPlanetRadius()!= radius) {
		shapeSettings.SetPlanetRadius(radius);
		RegeneratePlanet();
	}
}

void Planet::SetSeedForNoise(int seed)
{
	noise.ChangeDitribution(seed);
	RegeneratePlanet();
}

void Planet::RegeneratePlanet()
{
	ShapeGenerator generator(shapeSettings, this->noise);
	for (int i = 0; i < 6; i++)
		if (faces[i] != nullptr && renderFace[i])
			faces[i]->UpdateMembers(generator);
}

void Planet::Render() const {
	for (int i = 0; i < 6; i++)
		if (faces[i] != nullptr && renderFace[i])
			faces[i]->Render();
}