#ifndef TERRAIN_VERTEX_H
#define TERRAIN_VERTEX_H

#include <glm/glm.hpp>

struct TerrainVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    TerrainVertex() : position(0.0f), normal(0.0f), texCoord(0.0f) {}
};

#endif // !TERRAIN_VERTEX_H
