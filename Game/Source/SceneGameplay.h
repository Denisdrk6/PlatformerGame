#ifndef __SCENE_GAMEPLAY_H__
#define __SCENE_GAMEPLAY_H__

#include "Module.h"
#include "Animation.h"
#include "List.h"
#include "Point.h"
#include "Scene.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

struct SDL_Texture;
class Collider;
struct FlyEnemy;
struct FloorEnemy;

class BlendedTexture
{
public:
	SDL_Texture* texture;
	int alpha;
	bool loaded = false;
	SDL_Rect rect;
};

class CheckPoint
{
public:
	bool activated = false;
	iPoint position = {0, 0};
	Collider* collider;
};

class Heart
{
public:
	bool activated = false;
	iPoint position = { 0, 0 };
	Collider* collider;
};


class Coin
{
public:
	bool activated = false;
	iPoint position = { 0, 0 };
	Collider* collider;
};

class SceneGameplay : public Scene
{
public:

	SceneGameplay();

	// Destructor
	virtual ~SceneGameplay();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool Unload();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

	BlendedTexture saveTexBlending;
	BlendedTexture loadTexBlending;

	SDL_Texture* pause = nullptr;
	SDL_Texture* settingsScreen = nullptr;

	CheckPoint checkPoints[2];

	Coin coins[7];
	Heart hearts;

	int points = 0;

	SDL_Texture* flags;
	SDL_Texture* img;

	List<FlyEnemy*> FlyEnemies;
	List<FloorEnemy*> FloorEnemies;

	char scoreText[10] = { "\0" };
	int totalTime = 0;
	int hiscore = 0;
	int scoreFont = -1;
	int timer=0;
	
	bool playerCol = false;

	bool toLoad = false;
	bool settings = false;

	//int hurtFx = 0;

private:
	SDL_Texture* heartsTex;
	SDL_Texture* coinsTex;
	SDL_Texture* iglu;

	Animation rotateCoin;
	Animation redCheckPoint;
	Animation greenCheckPoint;

	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnExit;
	GuiButton* btnTitle;

	GuiCheckBox* btnFullScreen;
	GuiCheckBox* btnVSync;

	GuiSlider* sldMusic;
	GuiSlider* sldFx;

	int currentCheckpoint = 0;

protected:
};

#endif // __SCENE_GAMEPLAY_H__