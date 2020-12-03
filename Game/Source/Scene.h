#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

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

	SDL_Texture* flags;

private:
	SDL_Texture*		img;
	SDL_Texture*		heart;
	SDL_Texture*		iglu;
	SDL_Texture*		coins;
	
	Animation			rotateCoin;
	Animation			redCheckPoint;
	Animation			greenCheckPoint;


	bool				playerCol = true;

protected:
};

#endif // __SCENE_H__