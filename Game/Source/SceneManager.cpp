#include "SceneManager.h"

#include "SceneGameplay.h"
#include "SceneIntro.h"
#include "SceneWin.h"

#include "App.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	1.0f
#define FADEIN_TRANSITION_SPEED		1.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex) : Module()
{
	name.Create("scenemanager");

	//current = new SceneIntro();

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->tex = tex;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	hoverFx = app->audio->LoadFx("Assets/Audio/Fx/hover.wav");
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/click.wav");

	current = new SceneIntro();
	current->Start();
	gameplay = new SceneGameplay();
	current->currentScene = SceneType::INTRO;
	current->Load(tex);

	next = nullptr;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{

	// L12b: Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	input->GetMousePosition(mouseX, mouseY);
	iPoint p = render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathfinding->CreatePath(origin, p, true);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}
	

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	bool ret = true;

	if (!onTransition)
	{
		if (current->currentScene != SceneType::GAMEPLAY || (onTransition == true && fadeOutCompleted == false))
			ret = current->Update(input, dt);

		else if (current->currentScene == SceneType::GAMEPLAY)
			ret = gameplay->Update(input, dt);
	}

	else
	{

		if (current->currentScene != SceneType::GAMEPLAY || (current->currentScene == SceneType::GAMEPLAY && fadeOutCompleted == false))
			ret = current->Update(input, dt);

		else if (current->currentScene == SceneType::GAMEPLAY && fadeOutCompleted == true)
		{
			ret = gameplay->Update(input, dt);
		}

		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f && next != nullptr)
			{
				transitionAlpha = 1.0f;

				current->Unload();	// Unload current screen
				next->Load(tex);	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;

				current->Start();
			}

			else if (transitionAlpha > 1.01f && next == nullptr)
			{
				current->Unload();	// Unload current screen
				fadeOutCompleted = true;
				transitionAlpha = 1.0f;
			}
		}

		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw(render);

	if (current->transitionRequired || gameplay->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			case SceneType::INTRO:
				current->currentScene = SceneType::INTRO;
				next = new SceneIntro();
				break;

			case SceneType::GAMEPLAY:
				current->currentScene = SceneType::GAMEPLAY;
				current->nextScene = SceneType::WIN;
				gameplay->Start();
				next = nullptr;
				break;

			case SceneType::WIN:
				gameplay->Unload();
				current->currentScene = SceneType::WIN;
				next = new SceneWin();
				break;

			default: break;
		}

		current->transitionRequired = false;
	}

	//if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
	return ret;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	//ret = current->PostUpdate();

	if (current->currentScene != SceneType::GAMEPLAY || (onTransition == true && fadeOutCompleted == false))
		ret = current->PostUpdate();

	else if (current->currentScene == SceneType::GAMEPLAY)
		ret = gameplay->PostUpdate();

	

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, (unsigned char)(255.0f * transitionAlpha) }, true, false);
	}

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) current->Unload();

	return true;
}
