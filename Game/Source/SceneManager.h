#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"
#include "SceneGameplay.h"

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

	SceneGameplay* gameplay;

	Scene* current;
	Scene* next;

	bool onTransition;

	bool fileAsked = false;

	int hoverFx = 0;
	int clickFx = 0;

private:

	Input* input;
	Render* render;
	Textures* tex;
	Map* map;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool fadeOutCompleted;
	float transitionAlpha;
};

#endif // __SCENEMANAGER_H__