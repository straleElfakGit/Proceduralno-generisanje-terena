#ifndef TERRAIN_FACE_H
#define TERRAIN_FACE_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Buffers/VBO.h"
#include "Buffers/VAOLayout.h"
#include "Buffers/EBO.h"
#include "PlanetVertex.h"
#include "ShapeGenerator.h"

class TerrainFace {
private:
	unsigned int resolution;
	glm::vec3 localUp;
	glm::vec3 axisA;
	glm::vec3 axisB;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	std::unique_ptr<VAOLayout> VAO;
	std::unique_ptr<VBO<PlanetVertex>> VBOPtr;
	std::unique_ptr<EBO<GLuint>> EBOPtr;

	void ResizeVectors();
	void CalculateNormals();

	void ConstructFace(const ShapeGenerator& generator);
	void UpdateVAO();

	void GeneratePoint(const glm::vec2& percent, int i, const ShapeGenerator& generator);

public:
	static std::unique_ptr<TerrainFace> CreateUniq(int resolution, const glm::vec3&, const ShapeGenerator& generator);

	TerrainFace(int resolution, const glm::vec3& localUp, const ShapeGenerator& generator);
	~TerrainFace();

	void Render() const;
	void UpdateMembers(const ShapeGenerator& generator);

	unsigned int GetVertexCount() const { return resolution * resolution; }
	unsigned int GetIndexCount() const { return indices.size(); }
	unsigned int GetTriangleCount() const { return indices.size() / 3; }

	unsigned int GetResolution() const { return resolution; }
	void SetResolution(unsigned int res, const ShapeGenerator& generator);
};

#endif // !TERRAIN_FACE_H
