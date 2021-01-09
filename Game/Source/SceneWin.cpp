#include "SceneWin.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Player.h"
#include "Log.h"
#include "Collisions.h"
#include "Window.h"

#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL.h"

SceneWin::SceneWin() : Scene()
{

}

SceneWin::~SceneWin()
{

}

// Load assets
bool SceneWin::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bg = app->tex->Load("Assets/Screens/win.png");
	winFx = app->audio->LoadFx("Assets/Audio/Fx/win.wav");

	app->player->score = 0;
	app->player->map = 1;
	if (app->player->lifes != 3) app->player->lifes = 3;

	if (app->player->active == true)
	{
		app->player->Disable();
	}
	if (app->col->active == true)
	{
		app->col->Disable();
	}

	return ret;
}

bool SceneWin::Update(Input* input, float dt)
{
	bool ret = true;
	app->audio->PlayFx(winFx);

	/*if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(this, (Module*)app->scene, 60);*/

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	// Draw everything ---
	app->render->DrawTexture(bg, app->render->camera.x * -1, app->render->camera.y * -1, NULL, 1);
	return true;
}


bool SceneWin::Unload()
{
	app->player->active = true;
	app->audio->active = true;
	app->map->active = true;
	app->col->active = true;

	return true;
}