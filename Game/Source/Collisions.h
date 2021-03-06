#ifndef __j1COLLISIONS_H__
#define _j1COLLISIONS_H_

#include "PugiXml/src/pugixml.hpp"
#include "List.h"
#include "Point.h"
#include "Map.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"

#define MAX_COLLIDERS 1000

//------------------------------------------------------------------------
enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_FLOOR,
	COLLIDER_WALL,
	COLLIDER_DEAD,
	COLLIDER_END,
	COLLIDER_WIN,
	COLLIDER_FINISH,
	COLLIDER_CHECKPOINT,
	COLLIDER_COIN,
	COLLIDER_HEART,
	COLLIDER_ENEMY,
	COLLIDER_GODMODE,
	COLLIDER_SHOT,
	COLLIDER_MAX
	
};
//------------------------------------------------------------------------
struct Collider
{
	SDL_Rect rect;
	bool toDelete = false;
	COLLIDER_TYPE type;
	Module* callback = nullptr;

	Collider(SDL_Rect rectC, COLLIDER_TYPE typeC, Module* callbackC = nullptr) {
		rect = rectC;
		type = typeC;
		callback = callbackC;
	}

	void SetPos(int, int);




	bool CheckCollision(const SDL_Rect& r) const;
};

//------------------------------------------------------------------------
class Collisions : public Module
{
public:

	Collisions();

	// Destructor
	virtual ~Collisions();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool PreUpdate();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool LoadColliders(pugi::xml_node& node);

	Collider* AddCollider(SDL_Rect rectC, COLLIDER_TYPE typeC, Module* callbackC = nullptr);
	void DebugDraw();
	void DeleteCollider(Collider*);

	bool debug = false;


	Collider* colliders[MAX_COLLIDERS];

private:
	//bool LoadColliders();
	SString folder;
	pugi::xml_document collisionsData;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	int	playerFloorCol;
	int	playerWallCol;
	int	playerDeadCol;
	int	playerEnemyCol;
};

#endif // __j1COLLISIONS_H__
