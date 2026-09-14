#include "Core/Application/Application.h"
#include "Core/Scenes/BasicScene.h"
#include "Core/Scenes/SceneMenu.h"
#include "Core/Scenes/TerrainScene.h"
#include <memory>

int main() 
{	
	ApplicationBase* app = new Application();
	
	Scene* currentScene = nullptr;
	SceneMenu* menu = new SceneMenu(app, currentScene);

	menu->RegisterScene<BasicScene>("Basic Scene", app);
	menu->RegisterScene<TerrainScene>("Terrain Scene", app);

	app->SetMenuScene(menu);
	app->Run();

	delete app;
	return 0;
}
