#ifndef __SCENE_WIN_H__
#define __SCENE_WIN_H__

#include "Module.h"
#include "Animation.h"
#include "Scene.h"

struct SDL_Texture;

class SceneWin : public Scene
{
public:
	//Constructor
	SceneWin();

	//Destructor
	~SceneWin();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(Input* input, float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool Unload();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	//BlendTexture bgTexture;
	SDL_Texture* bg;
	int winFx = 0;
};

#endif
