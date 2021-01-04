#include "SceneManager.h"

#include "App.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

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

	/*intro = new SceneIntro();
	gameplay = new SceneGameplay();
	win = new SceneWin();*/
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
	current = new SceneIntro();
	//current->currentScene = SceneType::INTRO;
	current->Load(tex);
	current->Start();

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
		//if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) render->camera.y -= 1;
		//if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) render->camera.y += 1;
		//if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) render->camera.x -= 1;
		//if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) render->camera.x += 1;

		current->Update(input, dt);
		/*if (current->currentScene == SceneType::INTRO)
		{
			ret = intro->Update(dt);
		}
		if (current->currentScene == SceneType::GAMEPLAY)
		{
			ret = gameplay->Update(dt);
		}
		if (current->currentScene == SceneType::WIN)
		{
			ret = win->Update(dt);
		}*/
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
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

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, (unsigned char)(255.0f * transitionAlpha) });
	}

	// L12b: Debug pathfinding
	/*
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = app->pathFinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
	}
	*/

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			case SceneType::INTRO:
				//intro = new SceneIntro();
				//current->currentScene = SceneType::INTRO;
				next = new SceneIntro();
				break;

			case SceneType::GAMEPLAY: 
				//gameplay = new SceneGameplay();
				//current->currentScene = SceneType::GAMEPLAY;
				next = new SceneGameplay();
				break;

			case SceneType::WIN:
				//win = new SceneWin();
				//current->currentScene = SceneType::WIN;
				next = new SceneWin();
				break;

			default: break;
		}

		current->transitionRequired = false;
	}

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	ret = current->PostUpdate();

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) current->Unload();

	return true;
}
