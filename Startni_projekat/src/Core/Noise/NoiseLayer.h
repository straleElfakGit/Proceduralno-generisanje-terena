#ifndef NOISE_LAYER_H
#define NOISE_LAYER_H

#include "NoiseSettings.h"

class NoiseLayer
{
private:
	NoiseSettings noiseSettings;
	bool enabled;
	bool useFirstLayerAsMask;

public:
	NoiseLayer(): noiseSettings(), enabled(true), useFirstLayerAsMask(true) {
		LOG_FUNC();
	}
	~NoiseLayer() {}

	NoiseSettings& GetNoiseSettings() { return noiseSettings; }
	const NoiseSettings& GetNoiseSettingsConst() const { return noiseSettings; }

	bool GetEnabled () const { return enabled; }
	bool& GetEnabledRef() { return enabled; }

	bool GetUseFirstLayerAsMask() const { return useFirstLayerAsMask; }
	bool& GetUseFirstLayerAsMaskRef() { return useFirstLayerAsMask; }
};

#endif // !NOISE_LAYER_H
