#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class BlendTexture
{
public:
	SDL_Texture* texture;
	int alpha;
	bool loaded = false;
	SDL_Rect rect;
};

class SceneIntro : public Module
{
public:
	//Constructor
	SceneIntro();

	//Destructor
	~SceneIntro();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	BlendTexture bgTexture;
	int waitTime = 1500;
	int initTime = 0;
	int alphaModifier = 1;
};

#endif