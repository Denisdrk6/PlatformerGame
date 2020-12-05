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

SceneWin::SceneWin() : Module()
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

	bg = app->tex->Load("Assets/screens/win.png");

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

bool SceneWin::Update(float dt)
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(this, (Module*)app->scene, 60);*/

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	// Draw everything ---
	app->render->DrawTexture(bg, app->render->camera.x + app->map->data.tileWidth * 2 + 8, app->render->camera.y, NULL);

	return true;
}


bool SceneWin::CleanUp()
{
	app->player->active = true;
	app->audio->active = true;
	app->map->active = true;
	app->col->active = true;

	return true;
}