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
#include "Pathfinding.h" 
#include "Entities.h"

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
	rotateCoin.speed = 0.02f;

	redCheckPoint.PushBack({ 0, 0, 23, 26 });
	redCheckPoint.PushBack({ 0, 26, 23, 26 });
	redCheckPoint.speed = 0.02f;

	greenCheckPoint.PushBack({ 25, 0, 23, 26 });
	greenCheckPoint.PushBack({ 25, 26, 23, 26 });
	greenCheckPoint.speed = 0.02f;
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
		img = app->tex->Load("Assets/textures/background.png");
		heartsTex = app->tex->Load("Assets/textures/heart.png");
		coinsTex = app->tex->Load("Assets/textures/coins.png");
		flags = app->tex->Load("Assets/textures/Check_points.png");
		iglu = app->tex->Load("Assets/textures/iglu.png");

		app->audio->PlayMusic("Assets/audio/music/Friends.ogg");
		app->audio->LoadFx("Assets/audio/fx/hurt_sound.wav");

		//WARNING: might be called when we change maps
		checkPoints[0].position = { 76, 88 };
		checkPoints[1].position = { 33, 44 };
		for (int i = 0; i < 2; i++)
		{
			if (checkPoints[i].position.x != 0 || checkPoints[i].position.y != 0)
				checkPoints[i].collider = app->col->AddCollider({ checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);
		}

		coins[0].position = { 3, 84};
		coins[1].position = { 43, 88 };
		coins[2].position = { 85, 95 };
		coins[3].position = { 86, 81 };
		coins[4].position = { 97, 62 };
		coins[5].position = { 68, 50 };
		coins[6].position = { 16, 39 };

		for (int i = 0; i < 7; i++)
		{
			if (coins[i].position.x != 0 || coins[i].position.y != 0)
				coins[i].collider = app->col->AddCollider({ coins[i].position.x * app->map->data.tileWidth, coins[i].position.y * app->map->data.tileHeight,app->map->data.tileWidth,app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_COIN, this);
		}

		hearts.position = { 85,74 };
		if (hearts.position.x != 0 || hearts.position.y != 0)
			hearts.collider = app->col->AddCollider({ hearts.position.x * app->map->data.tileWidth, hearts.position.y * app->map->data.tileHeight,app->map->data.tileWidth,app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, this);
		
		app->render->camera.y = -77.5 * 32;
	}
	int w, h;
	uchar* data = NULL;
	if (app->map->CreateWalkabilityMap(w, h, &data))
		app->pathfinding->SetMap(w, h, data);
	

	RELEASE_ARRAY(data);
	
	ListItem<ObjectLayer*>* ob_lay;
	for (ob_lay = app->map->data.obj_layers.start; ob_lay; ob_lay = ob_lay->next) {
		if (ob_lay->data->name == "Entities") {
			app->entity->LoadFromObjectLayer(ob_lay->data);
		}
	}
	return true;
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
		if (app->render->camera.x > -36) 
		{
			app->render->camera.x = -36;
		}

		if (app->render->camera.y < -2480) 
		{
			app->render->camera.y = -2480;
		}

		if (app->render->camera.x < (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1 + 32) + app->win->screenSurface->w) 
		{
			app->render->camera.x = (app->map->MapToWorld(app->map->data.width, app->map->data.height).x * -1 + 32) + app->win->screenSurface->w;
		}

		if (app->render->camera.y > 0) 
		{
			app->render->camera.y = 0;
		}
	}

	// Draw map
	app->render->DrawTexture(img, 0, 0, NULL);
	if (app->map->active == true) app->map->Draw();

	// Draw extras (coins, hearts, flags, iglu)
	

	for (int i = 0; i < 7; i++) 
	{
		switch (coins[i].activated)
		{
		case false:
			app->render->DrawTexture(coinsTex, coins[i].position.x* app->map->data.tileWidth, coins[i].position.y * app->map->data.tileHeight, &rotateCoin.GetCurrentFrame());

			break;
		case true:
			break;
		}
	}

	switch (hearts.activated)
	{
	case false:
		app->render->DrawTexture(heartsTex, hearts.position.x * app->map->data.tileWidth, hearts.position.y * app->map->data.tileHeight);

		break;
	case true:
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		switch (checkPoints[i].activated)
		{
		case false:
			if (checkPoints[i].position.x == 0 && checkPoints[i].position.y == 0)
				break;
			else app->render->DrawTexture(flags, checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 14, &redCheckPoint.GetCurrentFrame());
			break;
		case true:
			if (checkPoints[i].position.x == 0 && checkPoints[i].position.y == 0)
				break;
			else app->render->DrawTexture(flags, checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 14, &greenCheckPoint.GetCurrentFrame());
			break;
		}
	}

	app->render->DrawTexture(coinsTex, app->render->camera.x*-1 + 135, app->render->camera.y*-1 + 15, &rotateCoin.GetCurrentFrame());

	//Draw save or load textures
	if (saveTex.loaded == true)
	{
		saveTex.alpha--;
		if (saveTex.alpha <= 0)
		{
			//Unload texture if its completely transparent
			app->tex->UnLoad(saveTex.texture);
			saveTex.texture = nullptr;
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
			loadTex.texture = nullptr;
			loadTex.loaded = false;
		}

		else
		{
			SDL_SetTextureAlphaMod(loadTex.texture, loadTex.alpha);
			app->render->DrawTexture(loadTex.texture, 1050, 620, &loadTex.rect);
		}
	}

	if (app->player->map == 2)
		app->render->DrawTexture(iglu, 7 * app->map->data.tileWidth, 5 * app->map->data.tileWidth - (3 * app->map->data.tileWidth / 4) - 5, NULL);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d, FPS: %i , Av.FPS:%2i  Last Frame Ms:%02i", app->map->data.width, app->map->data.height, app->map->data.tileWidth, app->map->data.tileHeight, app->map->data.tilesets.Count(), app->FPS_n, app->FPS_a, app->Last_ms);
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
