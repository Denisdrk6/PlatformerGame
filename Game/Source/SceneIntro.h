#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"
#include "Scene.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class BlendTexture
{
public:
	SDL_Texture* texture = nullptr;
	int alpha = 0;
	bool loaded = false;
	SDL_Rect rect = {0, 0, 0, 0};
};

class SceneIntro : public Scene
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
	bool Update(Input* input, float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool Unload();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	// The scene sprite sheet loaded into an SDL_Texture
	BlendTexture bgTexture;
	SDL_Texture* opening = nullptr;
	SDL_Texture* creditsScreen = nullptr;
	SDL_Texture* SettingsScreen = nullptr;

	int waitTime = 500;
	int initTime = 0;
	int alphaModifier = 4;

	bool credits = false;
	bool settings = false;

	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnExit;

	GuiButton* btnCreditsBack;

	GuiCheckBox* btnFullScreen;
	GuiCheckBox* btnVSync;
};

#endif