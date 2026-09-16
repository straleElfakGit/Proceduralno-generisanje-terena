#ifndef PROCEDURAL_TEXTURE_2D_H
#define PROCEDURAL_TEXTURE_2D_H

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Noise/PerlinNoise.h"
#include "Noise/FractalNoise.h"


class ProceduralTexture2D
{
private:
    unsigned int width;
    unsigned int height;

    FractalNoise<float> fractalNoise;
    std::vector<float> values;

    float minValue;
    float maxValue;

public:
    static std::shared_ptr<ProceduralTexture2D> Create(unsigned int width, unsigned int height);
    
    ProceduralTexture2D(unsigned int width, unsigned int height);

    float operator[](uint32_t index) const { return values[index]; }
    float operator[](const glm::uvec2& coord) const {
        return values[coord.x * width + coord.y];
    }

    float At(size_t index) const {
        return values[std::min(index, values.size() - 1)];
    }

    void GenerateValues();

    void SetSize(const glm::uvec2& size);
    glm::uvec2 GetSize() const { return glm::uvec2(width, height); }

    std::vector<float>& GetValues() { return values; }
    const std::vector<float>& GetValues() const { return values; }
    float GetMinValue() const { return minValue; }
    float GetMaxValue() const { return maxValue; }

    void SetSeed(int seed) {
        fractalNoise.perlinNoise.SetSeed(seed);
        fractalNoise.perlinNoise.GeneratePermutations();
    }
    void SetOctaves(int octaves) { fractalNoise.octaveCount = octaves; }
    void SetScale(float scale);
    void SetOffset(float offset) { fractalNoise.offset = offset; }
    void SetGain(float gain) { fractalNoise.gain = gain; }
    void SetLacunarity(float lacunarity) { fractalNoise.lacunarity = lacunarity; }

    int GetSeed() const { return fractalNoise.perlinNoise.GetSeed(); }
    int GetOctaves() const { return fractalNoise.octaveCount; }
    float GetScale() const { return fractalNoise.scale; }
    float GetOffset() const { return fractalNoise.offset; }
    float GetGain() const { return fractalNoise.gain; }
    float GetLacunarity() const { return fractalNoise.lacunarity; }
};


#endif // !PROCEDURAL_TEXTURE_2D_H
