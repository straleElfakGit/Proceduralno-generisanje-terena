#ifndef TERRAIN_SCENE
#define TERRAIN_SCENE

#include "Scene.h"
#include <memory>
#include "TerrainGeneration/Terrain.h"
#include "shaderClass.h"
#include "Camera/Camera.h"
#include "Application/ApplicationBase.h"
#include "Logging/ErrorHandler.h"
#include "imgui/imgui.h"
#include <vector>
#include "TerrainGeneration/ProceduralTexture2D.h"
#include "Timer.h"

class TerrainScene : public Scene
{
private:
	std::unique_ptr<Terrain> terrainPtr;
	std::unique_ptr<Shader> shaderPtr;
    std::unique_ptr<Camera> cameraPtr;

    std::shared_ptr<ProceduralTexture2D> m_NoiseMap;

    Timer timer;              
    float sunVelocity = 0.1f;

    const glm::vec3 daySunColor = glm::vec3(1.2f, 1.1f, 0.9f);
    const glm::vec3 nightSunColor = glm::vec3(0.0f);
    const glm::vec3 dayAmbientColor = glm::vec3(0.35f, 0.4f, 0.45f);
    const glm::vec3 nightAmbientColor = glm::vec3(0.05f, 0.05f, 0.12f);

	float fov = 45.0f;

	virtual void OnScroll(double xoffset, double yoffset) override;

    std::vector<float> GenerateWaveHeightMap(unsigned int width, unsigned int height);

public:
    TerrainScene(ApplicationBase* app);

    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render() override;

    virtual void OnImGuiRender() override;
};

#endif // !TERRAIN_SCENE

