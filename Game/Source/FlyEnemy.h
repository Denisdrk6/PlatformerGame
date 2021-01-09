#ifndef __FLYENEMY_H__
#define __FLYENEMY_H__

#include "Entities.h"
#include "App.h"
#include "Animation.h"
//#include "Point.h"

class FlyEnemy : public Entity {
public:
	FlyEnemy(iPoint pos);
	~FlyEnemy();

	void Update(float dt);
	void Draw();
	void HandeInput();
	bool ChasePlayer(iPoint);
	void Reset();
	void Load(pugi::xml_node&);
	void Save(pugi::xml_node&) const;
	void OnCollision(Collider* c1, Collider* c2);
	void blitPath();

	Animation* CurrentAnimation;

	Animation idle;
	Animation hit;
	Animation death;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool hitted = false;
	bool dead = false;
	bool falling = false;

	iPoint initialPosition;
	iPoint vel;

private:
	enum states {
		NONE = 0,
		MOVE
	};
	states CurrentState;

	enum inputs {
		LEFT = 0,
		RIGHT,
		DOWN,
		UP,
		IN_MAX
	};
	inputs Currentinput;
	SDL_Texture* debugTex;
	float privateDt;
};

#endif