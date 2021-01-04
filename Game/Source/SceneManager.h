#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

#include "SceneIntro.h"
#include "SceneGameplay.h"
#include "SceneWin.h"

class GuiButton;

class Input;
class Render;
class Textures;
class Map;

class SceneManager : public Module
{
public:

	SceneManager(Input* input, Render* render, Textures* tex);

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SceneIntro* intro = nullptr;
	SceneGameplay* gameplay = nullptr;
	SceneWin* win = nullptr;

	Scene* current;
	Scene* next;

private:

	Input* input;
	Render* render;
	Textures* tex;
	Map* map;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;
};

#endif // __SCENEMANAGER_H__