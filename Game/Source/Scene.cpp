#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"

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
	app->map->Load("Devmap.tmx");
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
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();

		// Load texture parameters
		loadTex.rect = { 234, 0, 224, 83 };
		if (loadTex.loaded == false) loadTex.texture = app->tex->Load("Assets/textures/saveLoad.png");
		loadTex.alpha = 255;
		loadTex.loaded = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();

		// Save texture parameters
		saveTex.rect = { 0, 0, 224, 83 };
		if (saveTex.loaded == false) saveTex.texture = app->tex->Load("Assets/textures/saveLoad.png");
		saveTex.alpha = 255;
		saveTex.loaded = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->VolumeControl(4);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->VolumeControl(-4);

	if (app->player->collider->type == COLLIDER_GODMODE)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += 5;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= 5;

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += 5;

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= 5;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		playerCol = !playerCol;
		if (playerCol) app->player->collider->type = COLLIDER_GODMODE;
		else  app->player->collider->type = COLLIDER_PLAYER;
	}

	//Camera limits

	if (app->player->collider->type == COLLIDER_PLAYER)
	{
		if (app->render->camera.x > 0) {
			app->render->camera.x = 0;
		}

		if (app->render->camera.y < -2480) {
			app->render->camera.y = -2480;
		}

		if (app->render->camera.x < (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1) + app->win->screenSurface->w) {
			app->render->camera.x = (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1) + app->win->screenSurface->w;
		}

		if (app->render->camera.y > 0) {
			app->render->camera.y = 0;
		}
	}

	// Draw map
	app->render->DrawTexture(img, 0, 1470, NULL);

	app->map->Draw();

	//Draw save or load textures
	if (saveTex.loaded == true)
	{
		saveTex.alpha--;
		if (saveTex.alpha <= 0)
		{
			//Unload texture if its completely transparent
			app->tex->UnLoad(saveTex.texture);
			saveTex.loaded = false;
		}

		else
		{
			//Set alpha value to texture and render it
			SDL_SetTextureAlphaMod(saveTex.texture, saveTex.alpha);
			app->render->DrawTexture(saveTex.texture, 1050, 620, &saveTex.rect);
		}
	}

	if (loadTex.loaded == true)
	{
		loadTex.alpha--;
		if (loadTex.alpha <= 0)
		{
			app->tex->UnLoad(loadTex.texture);
			loadTex.loaded = false;
		}

		else
		{
			SDL_SetTextureAlphaMod(loadTex.texture, loadTex.alpha);
			app->render->DrawTexture(loadTex.texture, 1050, 620, &loadTex.rect);
		}
	}

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
