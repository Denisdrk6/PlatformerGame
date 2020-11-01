#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->map->Load("map.tmx");
	img = app->tex->Load("Assets/textures/BG.png");
	app->audio->PlayMusic("Assets/audio/music/Friends.ogg");
	app->render->camera.y = -77.5 * app->map->data.tileHeight;
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 5;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 5;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 5;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 5;

	//Camera limits
	if (app->render->camera.x > 0) {
		app->render->camera.x = 0;
	}

	if (app->render->camera.y < -2480) {
		app->render->camera.y = -2480;
	}

	if (app->render->camera.x < -800) {
		app->render->camera.x = -800;
	}

	if (app->render->camera.y > -1610) {
		app->render->camera.y = -1610;
	}

	// Draw map
	app->render->DrawTexture(img, 0, 1470, NULL);

	app->map->Draw();

	// L03: TODO 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", app->map->data.width, app->map->data.height, app->map->data.tileWidth, app->map->data.tileHeight, app->map->data.tilesets.count());
	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
