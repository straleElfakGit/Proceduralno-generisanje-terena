#include "TerrainFace.h"

#define TRIANGLES_PER_QUAD 2
#define INDICES_PER_TRIANGLE 3

std::unique_ptr<TerrainFace> TerrainFace::CreateUniq(int resolution, const glm::vec3& localUp, const ShapeGenerator& generator)
{
	return std::make_unique<TerrainFace>(resolution, localUp, generator);
}

TerrainFace::TerrainFace(int resolution, const glm::vec3& localUp, const ShapeGenerator& generator) :
	resolution(resolution), 
	localUp(localUp)
{
	axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	axisB = glm::cross(axisA, localUp);

	UpdateMembers(generator);
}

TerrainFace::~TerrainFace() { }

void TerrainFace::ResizeVectors()
{
	unsigned int vertexCount = GetVertexCount();
	positions.resize(vertexCount);
	
	normals.clear();
	normals.resize(vertexCount, glm::vec3(0.0f));
	
	unsigned int numberOfQuads = (resolution - 1) * (resolution - 1);
	unsigned int numberOfTriangles = numberOfQuads * TRIANGLES_PER_QUAD;
	unsigned int indicesCount = numberOfTriangles * INDICES_PER_TRIANGLE;
	indices.resize(indicesCount);
}

void TerrainFace::GeneratePoint(const glm::vec2& percent, int i, const ShapeGenerator& generator)
{
	float u = (percent.x - 0.5f) * 2.0f;
	float v = (percent.y - 0.5f) * 2.0f;

	glm::vec3 pA = u * axisA;
	glm::vec3 pB = v * axisB;

	glm::vec3 p = localUp + pA + pB;

	float x2 = p.x * p.x;
	float y2 = p.y * p.y;
	float z2 = p.z * p.z;

	glm::vec3 pointOnSphere;
	pointOnSphere.x = p.x * sqrt(1.0f - (y2 / 2.0f) - (z2 / 2.0f) + (y2 * z2 / 3.0f));
	pointOnSphere.y = p.y * sqrt(1.0f - (x2 / 2.0f) - (z2 / 2.0f) + (x2 * z2 / 3.0f));
	pointOnSphere.z = p.z * sqrt(1.0f - (x2 / 2.0f) - (y2 / 2.0f) + (x2 * y2 / 3.0f));

	positions[i] = generator.CalculatePointOnPlanet(pointOnSphere);
}

void TerrainFace::ConstructFace(const ShapeGenerator& generator)
{
	ResizeVectors();
	unsigned int maxCoord = resolution - 1;
	unsigned int triangleIndex = 0;

	int i = 0;
	for (unsigned int y = 0; y < resolution; y++) {
		for (unsigned int x = 0; x < resolution; x++) {
			
			glm::vec2 percent = glm::vec2(
				(float)x / maxCoord,
				(float)y / maxCoord);

			glm::vec3 pointOnFace = localUp + (percent.x - 0.5f) * 2 * axisA + (percent.y - 0.5f) * 2 * axisB;
			glm::vec3 pointOnSphere = glm::normalize(pointOnFace);
			positions[i] = generator.CalculatePointOnPlanet(pointOnSphere);

			if (x != maxCoord && y != maxCoord) {
				indices[triangleIndex++] = i;
				indices[triangleIndex++] = i + resolution;
				indices[triangleIndex++] = i + resolution + 1;

				indices[triangleIndex++] = i;
				indices[triangleIndex++] = i + resolution + 1;
				indices[triangleIndex++] = i + 1;
			}

			i++;
		}
	}
	CalculateNormals();
}

void TerrainFace::CalculateNormals()
{
	unsigned int indexCount = GetIndexCount();

	for (unsigned int i = 0; i < indexCount; i += INDICES_PER_TRIANGLE) {
		unsigned int vIndex1 = indices[i + 0];
		unsigned int vIndex2 = indices[i + 1];
		unsigned int vIndex3 = indices[i + 2];

		const glm::vec3& v0 = positions[vIndex1];
		const glm::vec3& v1 = positions[vIndex2];
		const glm::vec3& v2 = positions[vIndex3];

		glm::vec3 normal = glm::normalize(
			glm::cross(v1 - v0, v2 - v0)
		);

		normals[vIndex1] += normal;
		normals[vIndex2] += normal;
		normals[vIndex3] += normal;
	}

	for (glm::vec3& normal : normals) {
		float len = glm::length(normal);
		if (len > 0.0001f) {
			normal /= len;
		}
		else {
			normal = glm::normalize(localUp);
		}
	}
}

void TerrainFace::UpdateMembers(const ShapeGenerator& generator)
{
	ConstructFace(generator);
	UpdateVAO();
}

void TerrainFace::UpdateVAO()
{
	size_t vertexCount = GetVertexCount();
	std::vector<PlanetVertex> vertices(vertexCount);

	for (unsigned int i = 0; i < vertexCount; i++) {
		vertices[i].position = positions[i];
		vertices[i].normal = normals[i];
	}

	VAO = std::make_unique<VAOLayout>();
	VAO->Bind();
	VBOPtr = std::make_unique<VBO<PlanetVertex>>(vertices);
	EBOPtr = std::make_unique<EBO<unsigned int>>(indices);

	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);

	VAO->addBuffer(*VBOPtr, vbl);

	VAO->Unbind();
	VBOPtr->Unbind();
	EBOPtr->Unbind();
}

void TerrainFace::Render() const
{
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void TerrainFace::SetResolution(unsigned int res, const ShapeGenerator& generator)
{
	if (this->resolution != res) {
		this->resolution = res;
		UpdateMembers(generator);
	}
}