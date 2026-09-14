#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <vector>
#include <Buffers/VBO.h>
#include <Buffers/VAOLayout.h>
#include <Buffers/EBO.h>
#include <memory>

class Terrain
{
private:
	const std::vector<float>& heightMap;
	glm::uvec2 size{ 0 };

	float tileScale = 1.0f;
	float heightScale = 1.0f;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	std::unique_ptr<VAOLayout> VAO;
	std::unique_ptr<VBO<TerrainVertex>> VBOPtr;
	std::unique_ptr<EBO<GLuint>> EBOPtr;

	void GenerateVertexDate();

	void GenerateTexCoords();
	void GeneratePositions();
	void GenerateNormals();
	void GenerateIndices();

	void UpdateVAO();

	inline size_t safeIndex(size_t index) const { return glm::min(index, heightMap.size() - 1); }
	inline float GetHeight(size_t index) const { return heightMap[safeIndex(index)]; }
	inline float GetHeightScaled(size_t index) const { return GetHeight(index) * heightScale; }
	inline static unsigned int GetInArrayIndex(unsigned int x, unsigned int y, unsigned int sizeX) { return y * sizeX + x; }

public:

	static std::unique_ptr<Terrain> CreateUniq(
		const glm::uvec2& size,
		const std::vector<float>& heightMap);
	Terrain(const glm::uvec2& size, const std::vector<float>& heightMap);
	~Terrain();

	void Render() const;
	void UpdateMembers();

	void SetSize(const glm::vec2& size) { this->size = size; }
	void SetTileScale(float scale) { this->tileScale = scale; }
	void SetHeightScale(float scale) { this->heightScale = scale; }

	glm::uvec2 GetSize() const { return size; }
	glm::vec2 GetWorldSize() const 
	{
		return glm::vec2(
			(float)(size.x - 1) * tileScale,
			(float)(size.y - 1) * tileScale);
	}
	float GetTileScale() const { return tileScale; }
	float GetHeightScale() const { return heightScale; }

	unsigned int GetVertexCount() const { return size.x * size.y; }
	unsigned int GetIndexCount() const { return indices.size(); }
	unsigned int GetTriangleCount() const { return indices.size() / 3; }
};

#endif // !TERRAIN_H
