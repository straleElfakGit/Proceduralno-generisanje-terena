#ifndef COLOR_SETTINGS_H
#define COLOR_SETTINGS_H

#include <algorithm>
#include <vector>
#include <glm/glm.hpp>

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
};


struct ColorSettings 
{
	Gradient gradient;

	ColorSettings()
	{
		gradient.keys = {
			{ 0.00f, glm::vec3(0.05f, 0.15f, 0.55f) }, 
			{ 0.30f, glm::vec3(0.10f, 0.35f, 0.75f) }, 
			{ 0.32f, glm::vec3(0.85f, 0.80f, 0.55f) }, 
			{ 0.40f, glm::vec3(0.20f, 0.55f, 0.20f) }, 
			{ 0.75f, glm::vec3(0.45f, 0.35f, 0.25f) }, 
			{ 1.00f, glm::vec3(1.00f, 1.00f, 1.00f) }
		};
	}

};

#endif // #ifndef COLOR_SETTINGS_H
