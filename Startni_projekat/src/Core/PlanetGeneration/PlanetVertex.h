#ifndef PLANET_VERTEX_H
#define PLANET_VERTEX_H

#include <glm/glm.hpp>

struct PlanetVertex {
    glm::vec3 position;
    glm::vec3 normal;
    float biomePercent;
    float unscaledElevation;

    PlanetVertex() : position(0.0f), normal(0.0f), biomePercent(0.0f), unscaledElevation(0.0f) {}
};

#endif // !PLANET_VERTEX_H