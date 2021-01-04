#include "SceneIntro.h"

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

SceneIntro::SceneIntro() : Module()
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture.texture = app->tex->Load("Assets/Textures/logo_dev_team.png");
	bgTexture.alpha = 1;
	bgTexture.loaded = true;

	opening = app->tex->Load("Assets/Screens/title.png");
	
	app->player->score = 0;
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

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	// Alpha speed is the same for all fps
	if(alphaModifier >= 0) alphaModifier = (int)(120 / (1/dt));

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && bgTexture.loaded == false)
		app->fade->Fade(this, (Module*)app->scene, 60);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)

		ret = false;

	
	return ret;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	// Draw everything ---
	//app->render->DrawTexture(bgTexture.texture, app->render->camera.x * -1 + (app->win->screenSurface->w / 2) - 180, app->render->camera.y * -1 + (app->win->screenSurface->h / 4), NULL, 1);

	if (bgTexture.loaded == true)
	{
		bgTexture.alpha += alphaModifier;

		if (bgTexture.alpha >= 255)
		{
			bgTexture.alpha = 255;
			if (initTime == 0) initTime = SDL_GetTicks();
			int currentTime = SDL_GetTicks();
			if (currentTime - initTime >= waitTime) alphaModifier = alphaModifier * -1;
		}

		if (bgTexture.alpha <= 0)
		{
			app->tex->UnLoad(bgTexture.texture);
			bgTexture.loaded = false;
		}

		else
		{
			SDL_SetTextureAlphaMod(bgTexture.texture, bgTexture.alpha);
			app->render->DrawTexture(bgTexture.texture, app->render->camera.x * -1 + (app->win->screenSurface->w / 2) - 180, app->render->camera.y * -1 + (app->win->screenSurface->h / 4), NULL, 1);
		}
	}

	else
		app->render->DrawTexture(opening, 0, 0, NULL, 1);

	return true;
}


bool SceneIntro::CleanUp()
{
	app->player->active = true;
	app->audio->active = true;
	app->map->active = true;
	app->col->active = true;
	app->render->camera.y = -77.5 * 32;

	return true;
}