#ifndef SHAPE_SETTINGS_H
#define SHAPE_SETTINGS_H

class ShapeSettings
{
private:
	float planetRadius = 1.0f;

public:
	ShapeSettings(float planetRadius): planetRadius(planetRadius) {}
	~ShapeSettings() {}

	float GetPlanetRadius() const { return planetRadius; }
	void SetPlanetRadius(float planetR) { this->planetRadius = planetR; }
};

#endif // #ifndef SHAPE_SETTINGS_H
