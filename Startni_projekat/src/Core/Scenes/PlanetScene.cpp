#include "PlanetScene.h"
#include "Logging/Logger.h"

PlanetScene::PlanetScene(ApplicationBase* app, float radius, unsigned int resolution) : 
	Scene(app), radius(radius), resolution(resolution)
{
	planetPtr = Planet::CreateUniq(radius, resolution);
	shaderPtr = std::make_unique<Shader>("assets/Shaders/planetShader.vert", "assets/Shaders/planetShader.frag");
	cameraPtr = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

	lightPtr = std::make_unique<DirectionalLight>(
		glm::vec3(0.2f, 0.2f, 0.25f),
		glm::vec3(1.0f, 0.95f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f));

	matPtr = std::make_unique<BasicMaterial>(
		glm::vec3(0.0f, 0.1f, 0.3f),
		glm::vec3(0.1f, 0.4f, 0.8f),
		glm::vec3(0.3f, 0.3f, 0.5f),
		16.0f);
}

void PlanetScene::Start()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void PlanetScene::UpdateSunPosition()
{
	float time = timer.ElapsedSeconds();
	float angle = time * sunVelocity;
	glm::vec3 sunPosition = glm::vec3(
		std::cos(angle) * sunDistance,
		sunHeight,
		std::sin(angle) * sunDistance
	);

	glm::vec3 sunDirection = glm::normalize(-sunPosition);
	lightPtr->UpdateDirection(sunDirection);
}

void PlanetScene::Update(float deltaTime)
{
	GLFWwindow* win = app->GetGLFWWindow();
	WindowData* data = (WindowData*)glfwGetWindowUserPointer(win);

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		cameraPtr->Inputs(win, deltaTime, data->width, data->height);


	UpdateSunPosition();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(radius));

	shaderPtr->Activate();
	
	shaderPtr->setMatrix("model", model);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
	shaderPtr->setMat3("normalMatrix", normalMatrix);

	cameraPtr->SetPositionToShader("viewPos", *shaderPtr);

	matPtr->SetShaderProgramParameters(*shaderPtr, "material");
	lightPtr->SetShaderProgramParameters(*shaderPtr, "dirLight");
}

void PlanetScene::Render()
{
	GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLFWwindow* win = app->GetGLFWWindow();
	WindowData* data = (WindowData*)glfwGetWindowUserPointer(win);
	cameraPtr->Matrix(fov, 0.1f, 1000.0f, *shaderPtr, "camMat", data->width, data->height);

	if (showMesh)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	planetPtr->Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PlanetScene::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Checkbox("Show Mesh", &showMesh);

	if (ImGui::Button("Randomize Seed"))
	{
		if (planetPtr != nullptr)
		{
			std::random_device rd;
			int newSeed = static_cast<int>(rd());
			planetPtr->SetSeedForNoise(newSeed);
		}
	}

	int currentRes = static_cast<int>(resolution);
	if (ImGui::SliderInt("Resolution", &currentRes, 2, 256))
	{
		resolution = static_cast<unsigned int>(currentRes);
		if (planetPtr != nullptr)
			planetPtr->SetResolution(resolution);
	}

	float currentRadius = radius;
	if (ImGui::SliderFloat("Radius", &currentRadius, 1.0f, 10.0f)) 
	{
		radius = currentRadius;
		LOG_INFO("Radius set to {}", radius);
		//if (planetPtr != nullptr)
			//planetPtr->SetRadius(radius);
	}


	ImGui::SliderFloat("Sun Velocity", &sunVelocity, 0.0f, 3.0f);
	ImGui::SliderFloat("Sun Height", &sunHeight, -10.0f, 10.0f);
	ImGui::SliderFloat("Sun Distance", &sunDistance, 1.0f, 50.0f);

	if (ImGui::Button("Reset Time"))
	{
		timer.Reset();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void PlanetScene::OnScroll(double xoffset, double yoffset)
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