#include "SceneManager.h"

#include "SceneIntro.h"
#include "GuiManager.h"

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

SceneIntro::SceneIntro() : Scene()
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

	creditsScreen = app->tex->Load("Assets/Screens/credits.png");
	SettingsScreen = app->tex->Load("Assets/Screens/settings.png");
	
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

	//startButton = app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//startButton->bounds();

	btnStart = new GuiButton(1, { 153, 194, 331, 76 }, "START");
	btnStart->SetObserver(this);

	btnContinue = new GuiButton(2, { 819, 194, 331, 76 }, "CONTINUE");
	btnContinue->SetObserver(this);
	app->sceneManager->fileAsked = true;
	app->LoadGameRequest();

	btnSettings = new GuiButton(3, { 153, 361, 331, 76 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 819, 361, 331, 76 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 567, 437, 147, 76 }, "EXIT");
	btnExit->SetObserver(this);

	btnCreditsBack = new GuiButton(6, { 32, 163, 141, 340 }, "CREDITS BACK");
	btnCreditsBack->SetObserver(this);

	btnFullScreen = new GuiCheckBox(7, { 302, 526, 30, 30 }, "FULL SCREEN");
	btnFullScreen->state = GuiControlState::NORMAL;
	btnFullScreen->checked = false;
	btnFullScreen->SetObserver(this);

	btnVSync = new GuiCheckBox(8, { 302, 621, 30, 30 }, "VSYNC");
	btnVSync->state = GuiControlState::NORMAL;
	btnVSync->checked = false;
	btnVSync->SetObserver(this);

	return ret;
}

bool SceneIntro::Update(Input* input, float dt)
{
	bool ret = true;

	if (credits == false)
	{
		ret = btnStart->Update(input, dt, false, app->render);
		ret = btnContinue->Update(input, dt, false, app->render);
		ret = btnSettings->Update(input, dt, false, app->render);
		ret = btnCredits->Update(input, dt, false, app->render);
		ret = btnExit->Update(input, dt, false, app->render);

		if (settings == true && ret == true)
		{
			ret = btnFullScreen->Update(input, dt);
			ret = btnVSync->Update(input, dt);
		}
	}

	else
		ret = btnCreditsBack->Update(input, dt, false, app->render);

	// Alpha speed is the same for all fps
	if(alphaModifier >= 0) alphaModifier = (int)(120 / (1/dt));

	//if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && bgTexture.loaded == false)
	//	TransitionToScene(SceneType::GAMEPLAY);
	//	//app->sceneManager->current->transitionRequired == true;
	//	//app->fade->Fade(this, (Module*)app->gameplay, 60);

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	
	return ret;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	// Draw everything ---
	
	// if there is no saved game and the continue button isn't disabled, disable it
	if (app->savedGameExists == false && btnContinue->state != GuiControlState::DISABLED) btnContinue->state = GuiControlState::DISABLED;

	// if there is a saved game and the contibue button is disabled, enable it
	else if (app->savedGameExists == true && btnContinue->state == GuiControlState::DISABLED) btnContinue->state = GuiControlState::NORMAL;

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
	{
		if (credits == false)
		{
			app->render->DrawTexture(opening, 0, 0, NULL, 1);

			btnStart->Draw(app->render);
			btnContinue->Draw(app->render);
			btnSettings->Draw(app->render);
			btnCredits->Draw(app->render);
			btnExit->Draw(app->render);

			if (settings == true)
			{
				app->render->DrawTexture(SettingsScreen, 0, 452, NULL, 1);

				btnFullScreen->Draw(app->render);
				btnVSync->Draw(app->render);
			}
		}

		else
		{
			app->render->DrawTexture(creditsScreen, 0, 0, NULL, 1);

			btnCreditsBack->Draw(app->render);
		}
	}

	return true;
}

bool SceneIntro::Unload()
{
	app->player->active = true;
	app->audio->active = true;
	app->map->active = true;
	app->col->active = true;
	app->render->camera.y = -77.5 * 32;
	app->tex->UnLoad(opening);

	return true;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);

		else if (control->id == 2)
		{
			TransitionToScene(SceneType::GAMEPLAY);
			app->sceneManager->gameplay->toLoad = true;
		}

		else if (control->id == 3) settings = !settings;

		else if (control->id == 4)
		{
			credits = true;

			btnStart->state = GuiControlState::NORMAL;
			btnContinue->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			btnCredits->state = GuiControlState::NORMAL;
			btnExit->state = GuiControlState::NORMAL;
		}

		else if (control->id == 5) return false; // TODO: Exit request

		else if (control->id == 6) credits = false;

		break;
	}

	case GuiControlType::CHECKBOX:

		if (control->id == 7)
		{
			if (btnFullScreen->checked == true) SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);

			else SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_MAXIMIZED);
		}

		break;

	default: break;
	}

	return true;
}