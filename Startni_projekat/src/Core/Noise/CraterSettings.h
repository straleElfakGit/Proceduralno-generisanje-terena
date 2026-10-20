#ifndef CRATER_SETTINGS_H
#define CRATER_SETTINGS_H

struct CraterSettings
{
	int seed = 0;
	int numberOfCraters = 100;

	float minRadius = 0.03f;
	float maxRadius = 0.20f;
	float sizeSkew = 3.0f;       

	float floorHeight = -0.5f;  
	float rimWidth = 0.7f;      
	float rimSteepness = 0.35f;
	float smoothness = 0.3f;
};

#endif //!CRATER_SETTINGS_H