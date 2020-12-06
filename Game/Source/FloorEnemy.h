#ifndef __FLOORENEMY_H__
#define __FLOORENEMY_H__

#include "Entities.h"
#include "Animation.h"

class FloorEnemy : public Entity {
public:
	FloorEnemy(iPoint pos);
	~FloorEnemy();

	void PreUpdate(float dt);
	void Update(float dt);
	void Draw();
	void HandeInput();
	bool ChasePlayer(fPoint);
	void Reset();
	void Load(pugi::xml_node&);
	void Save(pugi::xml_node&) const;
	void OnCollision(Collider* c1, Collider* c2);

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


	iPoint initialPosition;
	iPoint vel;

	bool dead = false;
	bool hitted = false;
	bool falling = true;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	float private_dt;
	//---------------------------------
	Animation* Current_animation;
	Animation idle;
	Animation hit;
	Animation death;

};

#endif
