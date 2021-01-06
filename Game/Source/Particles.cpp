#include "Particles.h"
#include "Textures.h"
#include "Scene.h"
#include "Player.h"
#include "Collisions.h"
#include "App.h"

#define SPEED 500

Particle::Particle(iPoint pos, int x, int y) : Entity(EntityType::particle) {
	sprite = app->tex->Load("Assets/textures/bullet.png");

	idle.PushBack({ 57,57,36,36 });
	idle.PushBack({ 57,57,36,36 });
	idle.speed = 7.0f;

	diferential_y = y - pos.y;
	diferential_x = x - pos.x;


	destination.x = x + 33;
	destination.y = y + 15;
	position = pos;

	col = app->col->AddCollider({ position.x,position.x,30,30 }, COLLIDER_SHOT, app->entities);
	player_y = app->player->position.y;

	lives = 1;
}

void Particle::Reset() {
	app->entities->DestroyEntity(this);
	app->col->DeleteCollider(col);
}

void Particle::Update(float dt) {


	if (position == destination) {
		lives = 0;
	}
	if (lives == 0) {
		app->entities->DestroyEntity(this);
		app->col->DeleteCollider(col);
	}

	if (diferential_x > 0) {
		position.x += SPEED * dt;
		if (diferential_y > 0) {
			float s = (float)diferential_y / diferential_x;
			if (app->fpsCapped == true) {
				position.y += s * SPEED * dt;
			}
			else {
				position.y = (s * position.x) + player_y;
			}
		}
		else if (diferential_y < 0) {
			int aux1_bis;
			aux1_bis = diferential_y * -1;
			float s = (float)aux1_bis / diferential_x;
			if (app->fpsCapped == true) {
				position.y += -s * SPEED * dt;
			}
			else {
				position.y = (-s * position.x) + player_y;
			}
		}
	}
	else if (diferential_x < 0) {
		position.x -= SPEED * dt;
		if (diferential_y > 0) {
			float s = (float)diferential_y / diferential_x;
			if (app->fpsCapped == true) {
				position.y += -s * SPEED * dt;
			}
			else {
				position.y = (-s * position.x) + player_y;
			}

		}
		else if (diferential_y < 0) {
			int aux1_bis;
			aux1_bis = diferential_y * -1;
			float s = (float)aux1_bis / diferential_x;
			if (app->fpsCapped == true) {
				position.y += s * SPEED * dt;
			}
			else {
				position.y = (s * position.x) + player_y;
			}
		}
	}

	col->SetPos(position.x, position.y);
	Draw();
}

void Particle::Draw() {
	float angle = 0.0f;
	if (diferential_x > 0) {
		if (diferential_y > 0) {
			float s = (float)diferential_y / diferential_x;
			angle = s * 40;
		}
		else if (diferential_y < 0) {
			int aux1_bis;
			aux1_bis = diferential_y * -1;
			float s = (float)aux1_bis / diferential_x;
			angle = s * -40;
		}
	}
	else if (diferential_x < 0) {
		if (diferential_y > 0) {
			float s = (float)diferential_y / diferential_x;
			angle = (s * 40) + 180;
		}
		else if (diferential_y < 0) {
			int aux1_bis;
			aux1_bis = diferential_y * -1;
			float s = (float)aux1_bis / diferential_x;
			angle = (s * -40) + 180;
		}
	}

	app->render->DrawTexture(sprite, position.x, position.y, &idle.GetCurrentFrame(), 1.0f, angle);
}

void Particle::OnCollision(Collider* c1, Collider* c2) {
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_WALL || c2->type == COLLIDER_ENEMY) {
		lives = 0;
	}

}