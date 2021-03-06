#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGameplay.h"
#include "SceneManager.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "Pathfinding.h" 
#include "Entities.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

SceneGameplay::SceneGameplay() : Scene()
{
	name.Create("gameplay");
	active = true;

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
SceneGameplay::~SceneGameplay()
{}

// Called before render is available
bool SceneGameplay::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneGameplay::Start()
{
	if (active == true)
	{
		app->map->Load("devmap.tmx");
		img = app->tex->Load("Assets/Textures/background.png");
		heartsTex = app->tex->Load("Assets/Textures/heart.png");
		coinsTex = app->tex->Load("Assets/Textures/coins.png");
		flags = app->tex->Load("Assets/Textures/check_points.png");
		iglu = app->tex->Load("Assets/Textures/iglu_icon.png");
		saveTexBlending.texture = app->tex->Load("Assets/textures/save_load.png");
		loadTexBlending.texture = app->tex->Load("Assets/Textures/Save_load.png");
		settingsScreen = app->tex->Load("Assets/Screens/settings.png");

		pause = app->tex->Load("Assets/Screens/pause.png");

		char lookupTable[] = { "0123456789" };
		scoreFont = app->fonts->Load("Assets/Fonts/timer_font.png", lookupTable, 1);


		app->audio->PlayMusic("Assets/Audio/Music/friends.ogg");
		//hurtFx = app->audio->LoadFx("Assets/Audio/Fx/hurt_sound.wav");

		//WARNING: might be called when we change maps
		checkPoints[0].position = { 76, 88 };
		checkPoints[1].position = { 33, 44 };
		for (int i = 0; i < 2; i++)
		{
			if (checkPoints[i].position.x != 0 || checkPoints[i].position.y != 0)
				checkPoints[i].collider = app->col->AddCollider({ checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, app->sceneManager);
		}

		coins[0].position = { 3, 84 };
		coins[1].position = { 43, 88 };
		coins[2].position = { 85, 95 };
		coins[3].position = { 86, 81 };
		coins[4].position = { 97, 62 };
		coins[5].position = { 68, 50 };
		coins[6].position = { 16, 39 };

		for (int i = 0; i < 7; i++)
		{
			if (coins[i].position.x != 0 || coins[i].position.y != 0)
				coins[i].collider = app->col->AddCollider({ coins[i].position.x * app->map->data.tileWidth, coins[i].position.y * app->map->data.tileHeight,app->map->data.tileWidth,app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_COIN, app->sceneManager);
		}

		hearts.position = { 85,74 };
		if (hearts.position.x != 0 || hearts.position.y != 0)
			hearts.collider = app->col->AddCollider({ hearts.position.x * app->map->data.tileWidth, hearts.position.y * app->map->data.tileHeight,app->map->data.tileWidth,app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, app->sceneManager);

		app->render->camera.y = -77.5 * 32;

		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);


		RELEASE_ARRAY(data);

		ListItem<ObjectLayer*>* obLay;
		for (obLay = app->map->data.objLayers.start; obLay; obLay = obLay->next)
		{
			if (obLay->data->name == "Entities") {
				app->entities->LoadFromObjectLayer(obLay->data);
			}
		}

		btnResume = new GuiButton(11, { 72, 78, 230, 60 }, "RESUME");
		btnResume->SetObserver(this);

		btnSettings = new GuiButton(12, { 72, 166, 230, 60 }, "SETTINGS");
		btnSettings->SetObserver(this);

		btnExit = new GuiButton(13, { 72, 250, 230, 60 }, "EXIT");
		btnExit->SetObserver(this);

		btnTitle = new GuiButton(14, { 72, 349, 230, 118 }, "TITLE");
		btnTitle->state = GuiControlState::DISABLED;
		btnTitle->SetObserver(this);

		btnFullScreen = new GuiCheckBox(15, { 302, 526, 30, 30 }, "FULL SCREEN");
		btnFullScreen->state = GuiControlState::NORMAL;
		btnFullScreen->checked = false;
		btnFullScreen->SetObserver(this);

		btnVSync = new GuiCheckBox(16, { 302, 621, 30, 30 }, "VSYNC");
		btnVSync->state = GuiControlState::DISABLED;
		btnVSync->checked = false;
		btnVSync->SetObserver(this);

		sldMusic = new GuiSlider(17, { 37, 526, 157, 30 }, "MUSIC");
		sldMusic->state = GuiControlState::NORMAL;
		sldMusic->maxValue = app->audio->maxMusicValue;
		sldMusic->minValue = 0;
		sldMusic->SetObserver(this);

		sldFx = new GuiSlider(18, { 37, 621, 157, 30 }, "MUSIC");
		sldFx->state = GuiControlState::NORMAL;
		sldFx->maxValue = app->audio->maxFxValue;
		sldFx->minValue = 0;
		sldFx->SetObserver(this);
	}

	app->pauseMenu = false;

	return true;
}

// Called each loop iteration
bool SceneGameplay::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGameplay::Update(Input* input, float dt)
{
	bool ret = true;

	if (app->pauseMenu == true)
	{
		if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		{
			btnResume->debugDraw = !btnResume->debugDraw;
			btnTitle->debugDraw = !btnTitle->debugDraw;
			btnSettings->debugDraw = !btnSettings->debugDraw;
			btnExit->debugDraw = !btnExit->debugDraw;
			btnFullScreen->debugDraw = !btnFullScreen->debugDraw;
			btnVSync->debugDraw = !btnVSync->debugDraw;
			sldMusic->debugDraw = !sldMusic->debugDraw;
			sldFx->debugDraw = !sldFx->debugDraw;
		}

		ret = btnResume->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
		ret = btnSettings->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
		ret = btnExit->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
		//if(ret == true) ret = btnTitle->Update(input, dt, true, app->render);
		if (settings == true && ret == true)
		{
			ret = btnFullScreen->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
			ret = btnVSync->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
			ret = sldMusic->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
			ret = sldFx->Update(input, dt, true, app->render, app->audio, app->sceneManager->hoverFx, app->sceneManager->clickFx);
		}
	}

	//If we prees continue button on title screen
	if (toLoad == true)
	{
		app->LoadGameRequest();
		toLoad = false;
	}

	if (app->pauseMenu == false && app->player->loadingScreen.loaded == false)
	{
		if (timer < 60)
		{
			timer++;

		}
		if (timer == 60)
		{
			totalTime = totalTime + 1;
			timer = 0;
		}
	}


    // L02: DONE 3: Request Load / Save when pressing L/S
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();

		// Load texture parameters
		loadTexBlending.rect = { 234, 0, 224, 83 };
		loadTexBlending.alpha = 255;
		loadTexBlending.loaded = true;
	}

	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();

		// Save texture parameters
		saveTexBlending.rect = { 0, 0, 224, 83 };
		saveTexBlending.alpha = 255;
		saveTexBlending.loaded = true;
	}

	if (input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->MusicVolumeControl(4);

	if (input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->MusicVolumeControl(-4);

	if (app->player->collider->type == COLLIDER_GODMODE)
	{
		if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += 5;

		if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= 5;

		if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += 5;

		if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= 5;
	}

	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		playerCol = !playerCol;
		if (playerCol) app->player->collider->type = COLLIDER_GODMODE;
		else  app->player->collider->type = COLLIDER_PLAYER;
	}

	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) // FPS cap to 30
	{
		if (app->maxFramerate != 30) {
			app->ChangeFrameCap(30);
		}
		else if (app->maxFramerate == 30) {
			app->ChangeFrameCap(60);
		}
	}

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && app->player->map != 1)
	{
		app->player->ChangeMap(1);
	}

	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && app->player->map != 2)
	{
		app->player->ChangeMap(2);
	}

	if (input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->player->ChangeMap(app->player->map);
		
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		if (currentCheckpoint == 0 || currentCheckpoint == 2 || app->player->map == 2)
		{
			app->player->position.x = checkPoints[0].position.x * 32;
			app->player->position.y = checkPoints[0].position.y * 32;

			if (app->player->map == 1)
			{
				app->render->camera.x = -1891;
				app->render->camera.y = -2480;
			}

			else if (app->player->map == 2)
			{
				app->render->camera.x = -1888;
				app->render->camera.y = -1061;
			}

			currentCheckpoint = 1;
		}

		else if (currentCheckpoint == 1 && app->player->map == 1)
		{
			app->player->position.x = checkPoints[1].position.x * 32;
			app->player->position.y = checkPoints[1].position.y * 32;

			app->render->camera.x = -577;
			app->render->camera.y = -1021;

			currentCheckpoint = 2;
		}
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
	app->render->DrawTexture(img, 0, 0, NULL, 1);
	if (app->map->active == true) app->map->Draw();

	// Draw extras (coins, hearts, flags, iglu)
	
	if (app->pauseMenu == false)
	{
		for (int i = 0; i < 7; i++)
		{
			switch (coins[i].activated)
			{
			case false:
				app->render->DrawTexture(coinsTex, coins[i].position.x * app->map->data.tileWidth, coins[i].position.y * app->map->data.tileHeight, &rotateCoin.GetCurrentFrame(), 1);

				break;
			case true:
				break;
			}
		}

		switch (hearts.activated)
		{
		case false:
			app->render->DrawTexture(heartsTex, hearts.position.x * app->map->data.tileWidth, hearts.position.y * app->map->data.tileHeight, NULL, 1);

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
				else app->render->DrawTexture(flags, checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 14, &redCheckPoint.GetCurrentFrame(), 2);
				break;
			case true:
				if (checkPoints[i].position.x == 0 && checkPoints[i].position.y == 0)
					break;
				else app->render->DrawTexture(flags, checkPoints[i].position.x * app->map->data.tileWidth, checkPoints[i].position.y * app->map->data.tileHeight + 14, &greenCheckPoint.GetCurrentFrame(), 2);
				break;
			}
		}

		app->render->DrawTexture(coinsTex, app->render->camera.x * -1 + 135, app->render->camera.y * -1 + 15, &rotateCoin.GetCurrentFrame(), 1);
	}

	//Draw save or load textures
	if (saveTexBlending.loaded == true)
	{
		saveTexBlending.alpha--;
		if (saveTexBlending.alpha <= 0)
		{
			//Unload texture if its completely transparent
			LOG("DESTROYING SAVETEX");
			//app->tex->UnLoad(saveTex);
			LOG("SAVETEX SCREEN DESTROYED");
			//saveTex = nullptr;
			saveTexBlending.loaded = false;
		}

		else
		{
			//Set alpha value to texture and render it
			int success = SDL_SetTextureAlphaMod(saveTexBlending.texture, saveTexBlending.alpha);

			if(success == 0)
				app->render->DrawTexture(saveTexBlending.texture, 1050, 620, &saveTexBlending.rect, 1);

			else
				LOG("Error aplying alpha to texture: %s\n", SDL_GetError());
		}
	}

	if (loadTexBlending.loaded == true)
	{
		loadTexBlending.alpha--;
		if (loadTexBlending.alpha <= 0)
		{
			//app->tex->UnLoad(loadTexBlending.texture);
			//loadTexBlending.texture = nullptr;
			loadTexBlending.loaded = false;
		}

		else
		{
			SDL_SetTextureAlphaMod(loadTexBlending.texture, loadTexBlending.alpha);
			app->render->DrawTexture(loadTexBlending.texture, 1050, 620, &loadTexBlending.rect, 1);
		}
	}

	if (app->player->map == 2)
		app->render->DrawTexture(iglu, 7 * app->map->data.tileWidth, 4 * app->map->data.tileWidth - (3 * app->map->data.tileWidth / 4) - 5, NULL, 1);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d, FPS: %i , Av.FPS:%2i  Last Frame Ms:%02i", app->map->data.width, app->map->data.height, app->map->data.tileWidth, app->map->data.tileHeight, app->map->data.tilesets.Count(), app->fpsN, app->fpsA, app->lastMs);
	app->win->SetTitle(title.GetString());
	/*sprintf_s(title, 256, "FPS: %i / Av.FPS: %.2f / Last Frame Ms: %02u (Frame Cap: %s) ",
		frames_on_last_update, avg_fps, last_frame_ms, frame_cap_title.GetString());*/
	return ret;
}

// Called each loop iteration
bool SceneGameplay::PostUpdate()
{
	bool ret = true;

	if (app->pauseMenu == false && app->player->lifes > 0)
	{
		//timer
		sprintf_s(scoreText, 10, "%d", totalTime);
		app->fonts->BlitText(550, 15, scoreFont, scoreText);
		sprintf_s(scoreText, 10, "%d", points);
		app->fonts->BlitText(10, 75, scoreFont, scoreText);
	}

	if (app->pauseMenu == true && app->player->active == true)
	{
		SDL_Rect rect = { 0, 0, app->win->screenSurface->w, app->win->screenSurface->h };
		SDL_SetRenderDrawColor(app->render->renderer, 255, 255, 255, 100);
		SDL_RenderFillRect(app->render->renderer, &rect);
		app->render->DrawTexture(pause, app->win->screenSurface->w / 3 + app->render->camera.x * -1, app->win->screenSurface->h / 5 + app->render->camera.y * -1);
	}

	if (app->pauseMenu == true)
	{
		btnResume->bounds = { app->win->screenSurface->w / 3 + app->render->camera.x * -1 + 72, app->win->screenSurface->h / 5 + app->render->camera.y * -1 + 78, 230, 60 };
		btnSettings->bounds = { app->win->screenSurface->w / 3 + app->render->camera.x * -1 + 72, app->win->screenSurface->h / 5 + app->render->camera.y * -1 + 166, 230, 60 };
		btnExit->bounds = { app->win->screenSurface->w / 3 + app->render->camera.x * -1 + 72, app->win->screenSurface->h / 5 + app->render->camera.y * -1 + 250, 230, 60 };
		btnTitle->bounds = { app->win->screenSurface->w / 3 + app->render->camera.x * -1 + 72, app->win->screenSurface->h / 5 + app->render->camera.y * -1 + 349, 230, 118 };

		btnResume->Draw(app->render);
		btnSettings->Draw(app->render);
		btnExit->Draw(app->render);
		btnTitle->Draw(app->render);

		if (settings == true)
		{
			btnFullScreen->bounds = { app->render->camera.x * -1 + 291, app->render->camera.y * -1 + app->win->screenSurface->h / 3 + 74, 30, 30 };
			btnVSync->bounds = { app->render->camera.x * -1 + 291, app->render->camera.y * -1 + app->win->screenSurface->h / 3 + 169, 30, 30 };
			sldMusic->bounds = { app->render->camera.x * -1 + 26, app->render->camera.y * -1 + app->win->screenSurface->h / 3 + 74, 157, 30 };
			sldFx->bounds = { app->render->camera.x * -1 + 26, app->render->camera.y * -1 + app->win->screenSurface->h / 3 + 169, 157, 30 };

			app->render->DrawTexture(settingsScreen, app->render->camera.x * -1 - 11, app->render->camera.y * -1 + app->win->screenSurface->h / 3, NULL, 1);

			btnFullScreen->Draw(app->render);
			btnVSync->Draw(app->render);
			sldMusic->Draw(true, app->render, app->input);
			sldFx->Draw(true, app->render, app->input);
		}
	}
	
	return ret;
}

// Called before quitting
bool SceneGameplay::Unload()
{
	LOG("Freeing scene");
	app->player->active = false;
	app->map->active = false;
	app->col->active = false;
	app->audio->MusicVolumeControl(0);
	settings = false;
	//app->entities->DestroyAll();

	return true;
}

bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 11)
		{
			app->pauseMenu = false;
			//settings = false;
		}

		else if (control->id == 12) settings = !settings;

		else if (control->id == 13) return false; // TODO: Exit request

		break;
	}

	case GuiControlType::CHECKBOX:
	{
		if (control->id == 15)
		{
			if (btnFullScreen->checked == true)
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);

			else
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_MAXIMIZED);
		}

		break;
	}

	case GuiControlType::SLIDER:
	{
		if (control->id == 17)
			app->audio->MusicVolumeControl(sldMusic->GetValue());

		if (control->id == 18)
			app->audio->FxVolumeControl(sldFx->GetValue());

		break;
	}

	default: break;
	}

	return true;
}