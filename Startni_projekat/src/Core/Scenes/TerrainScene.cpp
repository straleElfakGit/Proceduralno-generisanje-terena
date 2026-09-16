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

TerrainScene::TerrainScene(ApplicationBase* app) : Scene(app)
{
	m_NoiseMap = ProceduralTexture2D::Create(400, 400);
	m_NoiseMap->SetScale(220.0);

	m_NoiseMap->GenerateValues();

	glm::uvec2 terrainSize(400, 400);
	std::vector<float> testMap = GenerateWaveHeightMap(terrainSize.x, terrainSize.y);
	terrainPtr = Terrain::CreateUniq(terrainSize, m_NoiseMap->GetValues());
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

	float time = timer.ElapsedSeconds();
	float angle = time * sunVelocity;

	glm::vec3 sunPos;
	sunPos.x = std::cos(angle);
	sunPos.y = std::sin(angle);
	sunPos.z = 0.3f;

	glm::vec3 sunDir = glm::normalize(-sunPos);

	float dayFactor = glm::smoothstep(-0.1f, 0.2f, sunPos.y);

	glm::vec3 currentSunColor = glm::mix(nightSunColor, daySunColor, dayFactor);
	glm::vec3 currentAmbient = glm::mix(nightAmbientColor, dayAmbientColor, dayFactor);

	shaderPtr->Activate();
	shaderPtr->setVec3("sunDirection", sunDir);
	shaderPtr->setVec3("sunColor", currentSunColor);
	shaderPtr->setVec3("ambientColor", currentAmbient);
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
	ImGui::SliderFloat("Sun Velocity", &sunVelocity, 0.0f, 3.0f);

	if (ImGui::Button("Reset Time"))
	{
		timer.Reset();
	}

	float angle = timer.ElapsedSeconds() * sunVelocity;
	bool isDay = std::sin(angle) > 0.0f;
	ImGui::Text("Status: %s", isDay ? "Dan" : "Noc");
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
