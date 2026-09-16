#include "ProceduralTexture2D.h"

#include <memory>
#include <vector>
#include <limits>


std::shared_ptr<ProceduralTexture2D> ProceduralTexture2D::Create(unsigned int width, unsigned int height)
{
    return std::make_shared<ProceduralTexture2D>(width, height);
}

ProceduralTexture2D::ProceduralTexture2D(unsigned int width, unsigned int height)
    : width(width),
    height(height),
    fractalNoise(PerlinNoise<float>())
{
    //GenerateValues();
    //UpdateTexture();
}

void ProceduralTexture2D::GenerateValues()
{
    values.resize(width * height);

    minValue = std::numeric_limits<float>::max();
    maxValue = std::numeric_limits<float>::min();

    for (uint32_t y = 0; y < height; ++y)
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint32_t kIndex = y * width + x;
            const float kValue = fractalNoise.Noise(x, y, 0);

            values[kIndex] = kValue;

            minValue = glm::min(minValue, kValue);
            maxValue = glm::max(maxValue, kValue);
        }

    float range = maxValue - minValue;
    if (range < 0.00001f) range = 1.0f;

    for (size_t i = 0; i < values.size(); ++i)
    {
        values[i] = (values[i] - minValue) / range;
    }
}

void ProceduralTexture2D::SetSize(const glm::uvec2& size)
{
    width = size.x;
    height = size.y;
}

void ProceduralTexture2D::SetScale(float scale)
{
    scale = glm::max(0.001f, scale);
    fractalNoise.scale = scale;
}
