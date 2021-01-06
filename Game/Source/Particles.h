#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Entities.h"
#include "Animation.h"

class Particle : public Entity {
public:
	Particle(iPoint pos, int x, int y);
	~Particle() {}

	void Update(float dt);
	void Draw();
	void Reset();
	void OnCollision(Collider* c1, Collider* c2);

private:
	iPoint destination;
	int player_y;

	int diferential_y;
	int diferential_x;

	Animation idle;

};

#endif