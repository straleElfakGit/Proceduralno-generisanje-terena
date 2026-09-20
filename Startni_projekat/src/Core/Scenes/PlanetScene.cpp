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

	galaxyShaderPtr = std::make_unique<Shader>("assets/Shaders/skyBoxShader.vert", "assets/Shaders/skyBoxShader.frag");
	galaxyPtr = std::make_unique<Galaxy>(*galaxyShaderPtr);
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
	glm::vec3 sunPosition = glm::vec3(
		std::cos(sunAngle) * sunDistance,
		sunHeight,
		std::sin(sunAngle) * sunDistance);

	lightPtr->UpdateDirection(glm::normalize(-sunPosition));
}

void PlanetScene::Update(float deltaTime)
{
	GLFWwindow* win = app->GetGLFWWindow();
	WindowData* data = (WindowData*)glfwGetWindowUserPointer(win);

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		cameraPtr->Inputs(win, deltaTime, data->width, data->height);

	if (rotatePlanet)
		planetAngle += planetVelocity * deltaTime;
	else
		sunAngle += sunVelocity * deltaTime;

	planetAngle = std::fmod(planetAngle, glm::two_pi<float>());
	sunAngle = std::fmod(sunAngle, glm::two_pi<float>());

	UpdateSunPosition();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, planetAngle, glm::vec3(0.0f, 1.0f, 0.0f));
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

	glm::mat4 viewMat = cameraPtr->GetViewMatrix();
	glm::mat4 projectionMat = cameraPtr->GetProjectionMatrix(fov, 0.1f, 1000.0f, data->width, data->height);
	galaxyPtr->Render(viewMat, projectionMat);

	shaderPtr->Activate();
	cameraPtr->Matrix(fov, 0.1f, 1000.0f, *shaderPtr, "camMat", data->width, data->height);

	if (showMesh)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	planetPtr->Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PlanetScene::RenderNoiseGui(NoiseSettings& settings, int index)
{
	ImGui::PushID(index);

	if (ImGui::Button("Randomize Seed"))
	{
		if (planetPtr != nullptr)
		{
			std::random_device rd;
			int newSeed = static_cast<int>(rd());
			planetPtr->SetSeedForNoise(newSeed);
		}
	}
	
	bool changed = false;

	const char* filterTypeNames[] = { "Simple", "Rigid" };
	int currentType = static_cast<int>(settings.filterType);

	if (ImGui::Combo("Filter Type", &currentType, filterTypeNames, IM_ARRAYSIZE(filterTypeNames)))
	{
		settings.filterType = static_cast<FilterType>(currentType);
		changed = true;
	}

	changed |= ImGui::Checkbox("Enable layer", &planetPtr->GetLayerEnabledReference(index));
	changed |= ImGui::Checkbox("Use first layer as mask", &planetPtr->GetLayerUseFirstLayerAsMask(index));
	changed |= ImGui::SliderInt("Number of octavs", &settings.numberOfOctaves, 1, 8);
	changed |= ImGui::SliderFloat("Strength", &settings.strength, 0.0f, 2.0f);
	changed |= ImGui::SliderFloat("Base roughness", &settings.baseRoughness, 0.1f, 4.0f);
	changed |= ImGui::SliderFloat("Roughness", &settings.roughness, 1.0f, 4.0f);
	changed |= ImGui::SliderFloat("Persistance", &settings.persistance, 0.0f, 1.0f);
	changed |= ImGui::SliderFloat("Min value", &settings.minValue, 0.0f, 2.0f);
	changed |= ImGui::DragFloat3("Center", glm::value_ptr(settings.center), 0.01f);
	if (currentType == 1)
		changed |= ImGui::SliderFloat("Weight multiplier", &settings.weightMultiplier, 0.0f, 2.0f);

	if (changed)
		planetPtr->RegeneratePlanet();

	ImGui::PopID();
}

void PlanetScene::RenderFacesGui()
{
	if (planetPtr == nullptr) 
		return;

	if (ImGui::CollapsingHeader("Render Faces"))
	{
		const char* faceNames[6] = {
			"Top (+Y)",
			"Bottom (-Y)",
			"Left (-X)",
			"Right (+X)",
			"Front (+Z)",
			"Back (-Z)"
		};

		ImGui::PushID("RenderFacesGroup");

		for (int i = 0; i < 6; i++)
		{
			ImGui::Checkbox(faceNames[i], &planetPtr->GetRenderFaceRef(i));
		}

		ImGui::Separator();

		if (ImGui::Button("Select All"))
		{
			for (int i = 0; i < 6; i++)
				planetPtr->GetRenderFaceRef(i) = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Deselect All"))
		{
			for (int i = 0; i < 6; i++)
				planetPtr->GetRenderFaceRef(i) = false;
		}

		ImGui::PopID();
	}
}

void PlanetScene::RenderPlanetPropertiesGui()
{
	ImGui::Checkbox("Show Mesh", &showMesh);

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

	if (ImGui::Button("Add new noise layer"))
	{
		planetPtr->AddNewNoiseLayer();
	}
	RenderFacesGui();
}

void PlanetScene::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (ImGui::CollapsingHeader("Planet Settings"))
		RenderPlanetPropertiesGui();

	if (ImGui::CollapsingHeader("Sun & Lighting")) 
	{
		ImGui::SliderFloat("Sun Velocity", &sunVelocity, 0.0f, 3.0f);
		ImGui::SliderFloat("Sun Height", &sunHeight, -10.0f, 10.0f);
		ImGui::SliderFloat("Sun Distance", &sunDistance, 1.0f, 50.0f);

		ImGui::Checkbox("Rotate planet (freezes sun)", &rotatePlanet);
		ImGui::SliderFloat("Planet Velocity", &planetVelocity, 0.0f, 3.0f);

		if (ImGui::Button("Reset Angles"))
		{
			sunAngle = 0.0f;
			planetAngle = 0.0f;
		}
	}
	
	int numberOfLayers = planetPtr->GetNumberOfNoiseLayers();
	for (int i = 0; i < numberOfLayers; i++) {
		std::string headerLabel = "Noise Settings " + std::to_string(i + 1);
		if (ImGui::CollapsingHeader(headerLabel.c_str()))
		{
			if (planetPtr != nullptr)
				RenderNoiseGui(planetPtr->GetNoiseSettings(i), i);
		}
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