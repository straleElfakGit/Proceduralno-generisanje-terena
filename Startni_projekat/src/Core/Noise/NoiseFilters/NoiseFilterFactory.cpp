#include "NoiseFilterFactory.h"

std::vector<std::unique_ptr<NoiseFilter>> NoiseFilterFactoty::CreateNoiseFilters(Noise& noise, ShapeSettings& shapeSettings)
{
    std::vector<std::unique_ptr<NoiseFilter>> filters;
    filters.reserve(shapeSettings.GetNumberOfLayers());

    for (int i = 0; i < shapeSettings.GetNumberOfLayers(); ++i)
    {
        NoiseSettings& currentSettings = shapeSettings.GetNoiseSettings(i);

        if (currentSettings.filterType == FilterType::Simple)
        {
            filters.push_back(std::make_unique<SimpleNoiseFilter>(noise, currentSettings));
        }
        else if (currentSettings.filterType == FilterType::Rigid)
        {
            filters.push_back(std::make_unique<RigidNoiseFilter>(noise, currentSettings));
        }
    }

    return filters;
}
