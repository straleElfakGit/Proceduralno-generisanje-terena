#include "TerrainScene.h"

std::vector<float> TerrainScene::GenerateWaveHeightMap(unsigned int width, unsigned int height)
{
	std::vector<float> heightMap(width * height);

	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			float h = (std::sin(u * 3.14159f * 6.0f) + std::cos(v * 3.14159f * 6.0f) + 2.0f) * 0.25f;

			heightMap[y * width + x] = h;
		}
	}

	return heightMap;
}

std::vector<float> TerrainScene::GenerateIslandHeightMap(uint32_t width, uint32_t height)
{
	std::vector<float> heightMap(width * height);

	float centerX = width / 2.0f;
	float centerY = height / 2.0f;
	float maxRadius = std::min(centerX, centerY);

	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			float dx = (x - centerX) / maxRadius;
			float dy = (y - centerY) / maxRadius;
			float distance = std::sqrt(dx * dx + dy * dy);

			float h = std::max(0.0f, 1.0f - distance);

			heightMap[y * width + x] = h;
		}
	}

	return heightMap;
}

TerrainScene::TerrainScene(ApplicationBase* app) : Scene(app)
{
	glm::uvec2 terrainSize(400, 400);
	std::vector<float> testMap = GenerateWaveHeightMap(terrainSize.x, terrainSize.y);
	terrainPtr = Terrain::CreateUniq(terrainSize, testMap);
	terrainPtr->SetHeightScale(10.0f);
	terrainPtr->SetTileScale(0.125f);
	terrainPtr->UpdateMembers();
	shaderPtr = std::make_unique<Shader>("assets/Shaders/default.vert", "assets/Shaders/default.frag");
	cameraPtr = std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 2.0f));
}

void TerrainScene::Start()
{
	glEnable(GL_DEPTH_TEST);
}

void TerrainScene::Update(float deltaTime)
{
	GLFWwindow* win = app->GetGLFWWindow();
	WindowData* data = (WindowData*)glfwGetWindowUserPointer(win);

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		cameraPtr->Inputs(win, deltaTime, data->width, data->height);
}

void TerrainScene::Render()
{
	GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	glm::mat4 model = glm::mat4(1.0f);
	shaderPtr->Activate();
	shaderPtr->setMatrix("model", model);

	GLFWwindow* win = app->GetGLFWWindow();
	WindowData* data = (WindowData*)glfwGetWindowUserPointer(win);
	cameraPtr->Matrix(fov, 0.1f, 1000.0f, *shaderPtr, "camMat", data->width, data->height);

	terrainPtr->Render();
}

void TerrainScene::OnImGuiRender()
{
}

void TerrainScene::OnScroll(double xoffset, double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
