#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
class Collider;

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

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	BlendedTexture		saveTex;
	BlendedTexture		loadTex;

	CheckPoint			checkPoints[2];

	Coin				coins[7];
	Heart				hearts;


	SDL_Texture* flags;
	SDL_Texture* img;

private:
	SDL_Texture*		heartsTex;
	SDL_Texture*		coinsTex;
	SDL_Texture*		iglu;
	
	Animation			rotateCoin;
	Animation			redCheckPoint;
	Animation			greenCheckPoint;


	bool				playerCol = true;

protected:
};

#endif // __SCENE_H__