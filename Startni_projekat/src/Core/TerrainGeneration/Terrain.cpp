#include "Terrain.h"
#include "TerrainVertex.h"

#define TRIANGLES_PER_QUAD 2
#define INDICES_PER_TRIANGLE 3

std::unique_ptr<Terrain> Terrain::CreateUniq(const glm::uvec2& size, const std::vector<float>& heightMap)
{
	return std::make_unique<Terrain>(size, heightMap);
}

Terrain::Terrain(const glm::uvec2& size, const std::vector<float>& heightMap) :
	heightMap(heightMap), size(size)
{
	UpdateMembers();
}

Terrain::~Terrain() { }

void Terrain::UpdateMembers()
{
	GenerateVertexDate();
    UpdateVAO();
}


void Terrain::GenerateVertexDate()
{
	GenerateTexCoords();
	GeneratePositions();

	GenerateIndices();
	GenerateNormals();
}

void Terrain::GenerateTexCoords()
{
	size_t vertexCount = GetVertexCount();
	texCoords.resize(vertexCount);
	
	const unsigned int maxTexX = size.x - 1;
	const unsigned int maxTexY = size.y - 1;

	for (unsigned int y = 0; y < size.y; y++) {
		for (unsigned int x = 0; x < size.x; x++) {
			unsigned int index = Terrain::GetInArrayIndex(x, y, size.x);
			texCoords[index].x = (float)x / maxTexX;
			texCoords[index].y = (float)y / maxTexY;
		}
	}
}

void Terrain::GeneratePositions()
{
	const glm::vec2 worldSize = GetWorldSize();
	const glm::vec2 centerOffset = worldSize * 0.5f;

	size_t vertexCount = GetVertexCount();
	positions.resize(vertexCount);

	for (unsigned int y = 0; y < size.y; y++) {
		for (unsigned int x = 0; x < size.x; x++) {
			unsigned int index = Terrain::GetInArrayIndex(x, y, size.x);
			positions[index].x = texCoords[index].x * worldSize.x - centerOffset.x;
			positions[index].z = texCoords[index].y * worldSize.y - centerOffset.y;
			positions[index].y = GetHeightScaled(index);
		}
	}
}

void Terrain::GenerateIndices()
{
	size_t numberOfQuads = (size.x - 1) * (size.y - 1);
	size_t indicesCount = numberOfQuads * TRIANGLES_PER_QUAD * INDICES_PER_TRIANGLE;
	indices.resize(indicesCount);

	size_t index = 0;
	for (unsigned int y = 0; y < size.y - 1; y++) {
		for (unsigned int x = 0; x < size.x - 1; x++) {
			unsigned int vertexIndex = Terrain::GetInArrayIndex(x, y, size.x);

			indices[index++] = vertexIndex;
			indices[index++] = vertexIndex + size.x;
			indices[index++] = vertexIndex + size.x + 1;

			indices[index++] = vertexIndex;
			indices[index++] = vertexIndex + size.x + 1;
			indices[index++] = vertexIndex + 1;
		}
	}
}

void Terrain::GenerateNormals()
{
	size_t vertexCount = GetVertexCount();
	normals.clear();
	normals.resize(vertexCount, glm::vec3(0.0f));

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

	for (glm::vec3& normal : normals)
		normal = glm::normalize(normal);
}

void Terrain::UpdateVAO()
{
	size_t vertexCount = GetVertexCount();
	std::vector<TerrainVertex> vertices(vertexCount);

	for (unsigned int i = 0; i < vertexCount; i++) {
		vertices[i].position = positions[i];
		vertices[i].normal = normals[i];
		vertices[i].texCoord = texCoords[i];
	}
	
	VAO = std::make_unique<VAOLayout>();
	VAO->Bind();
	VBOPtr = std::make_unique<VBO<TerrainVertex>>(vertices);
	EBOPtr = std::make_unique<EBO<unsigned int>>(indices);

	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);

	VAO->addBuffer(*VBOPtr, vbl);

	VAO->Unbind();
	VBOPtr->Unbind();
	EBOPtr->Unbind();
}

void Terrain::Render() const
{
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}