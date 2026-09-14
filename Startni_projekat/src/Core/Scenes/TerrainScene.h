#ifndef TERRAIN_SCENE
#define TERRAIN_SCENE

#include "Scene.h"
#include <memory>
#include "TerrainGeneration/Terrain.h"
#include "shaderClass.h"
#include "Camera/Camera.h"
#include "Application/ApplicationBase.h"
#include "ErrorHandler.h"
#include "imgui/imgui.h"

class TerrainScene : public Scene
{
private:
	std::unique_ptr<Terrain> terrainPtr;
	std::unique_ptr<Shader> shaderPtr;
    std::unique_ptr<Camera> cameraPtr;
	float fov = 45.0f;

	virtual void OnScroll(double xoffset, double yoffset) override;

    std::vector<float> GenerateWaveHeightMap(unsigned int width, unsigned int height);
    std::vector<float> GenerateIslandHeightMap(unsigned int width, unsigned int height);
public:
    TerrainScene(ApplicationBase* app);

    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render() override;

    virtual void OnImGuiRender() override;
};

#endif // !TERRAIN_SCENE

