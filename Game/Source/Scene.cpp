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
	active = false;

	rotateCoin.PushBack({ 0,0,15,32 });
	rotateCoin.PushBack({ 18,0,24,32 });
	rotateCoin.PushBack({ 45,0,29,32 });
	rotateCoin.PushBack({ 76,0,31,32 });
	rotateCoin.PushBack({ 108,0,33,32 });
	rotateCoin.PushBack({ 144,0,32,32 });
	rotateCoin.PushBack({ 180,0,28,32 });
	rotateCoin.PushBack({ 212,0,24,32 });
	rotateCoin.PushBack({ 241,0,16,32 });
	rotateCoin.speed = 0.05f;
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
	if (active == true)
	{
		app->map->Load("Devmap.tmx");
		img = app->tex->Load("Assets/textures/BG.png");
		heart = app->tex->Load("Assets/textures/heart.png");
		coins = app->tex->Load("Assets/textures/coins.png");
		iglu = app->tex->Load("Assets/textures/iglu.png");

		app->audio->PlayMusic("Assets/audio/music/Friends.ogg");
		app->audio->LoadFx("Assets/audio/fx/hurt_sound.wav");


		
	
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	/*if (active == true)
	{
		if(app->player->active == false) app->player->active = true;
		if (app->audio->active == false) app->audio->active = true;
		if (app->map->active == false) app->map->active = true;
		if (app->col->active == false) app->col->active = true;
	}*/
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

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)

	//Camera limits

	if (app->player->collider->type == COLLIDER_PLAYER)
	{
		if (app->render->camera.x > -36) {
			app->render->camera.x = -36;
		}

		if (app->render->camera.y < -2480) {
			app->render->camera.y = -2480;
		}

		if (app->render->camera.x < (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1 + 32) + app->win->screenSurface->w) {
			app->render->camera.x = (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1 + 32) + app->win->screenSurface->w;
		}

		if (app->render->camera.y > 0) {
			app->render->camera.y = 0;
		}
	}

	// Draw map
	app->render->DrawTexture(img, 0, 1470, NULL);


	

	app->render->DrawTexture(heart, 85 * app->map->data.tileWidth, 74 * app->map->data.tileHeight, NULL);
	app->render->DrawTexture(heart, 5 * app->map->data.tileWidth, 26 * app->map->data.tileHeight, NULL);


	app->render-> DrawTexture(coins,20* app->map->data.tileWidth, 78*app->map->data.tileHeight, &rotateCoin.GetCurrentFrame());
	app->render->DrawTexture(coins, 21 * app->map->data.tileWidth, 78 * app->map->data.tileHeight, &rotateCoin.GetCurrentFrame());
	app->render->DrawTexture(coins, 22 * app->map->data.tileWidth, 78 * app->map->data.tileHeight, &rotateCoin.GetCurrentFrame());
	app->render->DrawTexture(coins, 23 * app->map->data.tileWidth, 78 * app->map->data.tileHeight, &rotateCoin.GetCurrentFrame());


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

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d, FPS: %i , Av.FPS:%2i  Last Frame Ms:%02i", app->map->data.width, app->map->data.height, app->map->data.tileWidth, app->map->data.tileHeight, app->map->data.tilesets.count(), app->FPS_n, app->FPS_a, app->Last_ms);
	app->win->SetTitle(title.GetString());
	/*sprintf_s(title, 256, "FPS: %i / Av.FPS: %.2f / Last Frame Ms: %02u (Frame Cap: %s) ",
		frames_on_last_update, avg_fps, last_frame_ms, frame_cap_title.GetString());*/
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
