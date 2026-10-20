#ifndef CRATER_NOIS_FILTER_H
#define CRATER_NOIS_FILTER_H

#include <vector>
#include <glm/glm.hpp>

#include "NoiseFilter.h"
#include "Noise/CraterSettings.h"

class CraterFilter : public NoiseFilter
{
private:
	struct Crater
	{
		glm::vec3 center;
		float radius;
		float influence2;
	};

	CraterSettings settings;
	float strength;
	std::vector<Crater> craters;

	static float SmoothMin(float a, float b, float k);
	static float SmoothMax(float a, float b, float k);

	void GenerateCraters();

	float CraterShape(float x) const;

public:
	CraterFilter(Noise& noise, NoiseSettings& settings);
	~CraterFilter();

	float Evaluate(const glm::vec3& point) override;

};

#endif // !CRATER_NOIS_FILTER_H
