#ifndef __FLYENEMT_H__
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

	Animation* Current_animation;

	Animation idle;
	Animation hit;
	Animation death;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool hitted = false;
	bool dead = false;
	bool falling = false;

	iPoint initial_position;
	iPoint vel;

	int maxLives;
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


	float private_dt;
};

#endif
