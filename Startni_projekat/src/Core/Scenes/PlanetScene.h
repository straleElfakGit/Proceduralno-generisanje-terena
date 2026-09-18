#ifndef PLANET_SCENE_H
#define PLANET_SCENE_H

#include <memory>
#include <vector>
#include <random>
#include "Scene.h"
#include "TerrainGeneration/Terrain.h"
#include "shaderClass.h"
#include "Camera/Camera.h"
#include "Application/ApplicationBase.h"
#include "Logging/ErrorHandler.h"
#include "imgui/imgui.h"
#include "Timer.h"
#include "PlanetGeneration/Planet.h"
#include "Lights/DirectionalLight.h"
#include "Materials/BasicMaterial.h"

class PlanetScene : public Scene
{
private:
    std::unique_ptr<Planet> planetPtr;
    std::unique_ptr<Shader> shaderPtr;
    std::unique_ptr<Camera> cameraPtr;

    std::unique_ptr<DirectionalLight> lightPtr;
    std::unique_ptr<BasicMaterial> matPtr;

    float radius;
    unsigned int resolution;

    Timer timer;
    float sunVelocity = 0.1f;
    float sunHeight = 2.0f;
    float sunDistance = 10.0f;

    bool showMesh = false;

    float fov = 45.0f;

    virtual void OnScroll(double xoffset, double yoffset) override;

    void UpdateSunPosition();

public:
    PlanetScene(ApplicationBase* app, float radius = 1.0f, unsigned int resolution = 4);

    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render() override;

    virtual void OnImGuiRender() override;
};

#endif // !PLANET_SCENE_H

