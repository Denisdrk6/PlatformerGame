#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class BlendedTexture
{
public:
	SDL_Texture* texture;
	int alpha;
	bool loaded = false;
	SDL_Rect rect;
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

	BlendedTexture saveTex;
	BlendedTexture loadTex;

private:
	SDL_Texture* img;
	bool playerCol = true;

protected:
};

#endif // __SCENE_H__