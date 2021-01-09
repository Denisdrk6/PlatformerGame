#include "Particles.h"
#include "Textures.h"
#include "Scene.h"
#include "Player.h"
#include "Collisions.h"
#include "App.h"
#include "Window.h"

#define SPEED 500

Particle::Particle(iPoint pos, int x, int y) : Entity(EntityType::particle)
{
	sprite = app->player->shot;
	
	idle.PushBack({ 57,57,36,36 });
	idle.PushBack({ 57,57,36,36 });
	idle.speed = 7.0f;

	diferentialY = y - pos.y;
	diferentialX = x - pos.x;

	destination.x = x + 33;
	destination.y = y + 15;
	position = pos;

	col = app->col->AddCollider({ position.x,position.x,30,30 }, COLLIDER_SHOT, app->entities);
	playerY = app->player->position.y;

	lives = 1;
}

void Particle::Reset()
{
	app->entities->DestroyEntity(this);
	app->col->DeleteCollider(col);
}

void Particle::Update(float dt)
{
	if (position == destination)
	{
		lives = 0;
	}

	if (position.x >= app->map->data.width * app->map->data.tileWidth || position.x < 0 || position.y >= app->map->data.height * app->map->data.tileHeight || position.y < 0)
	{
		lives = 0;
	}

	if (lives == 0)
	{
		app->entities->DestroyEntity(this);
		app->col->DeleteCollider(col);
	}

	if (diferentialX > 0)
	{
		position.x += SPEED * dt;
		if (diferentialY > 0) {
			float s = (float)diferentialY / diferentialX;

			if (app->fpsCapped == true)
			{
				position.y += s * SPEED * dt;
			}

			else
			{
				position.y = (s * position.x) + playerY;
			}
		}

		else if (diferentialY < 0)
		{
			int aux1;
			aux1 = diferentialY * -1;
			float s = (float)aux1 / diferentialX;

			if (app->fpsCapped == true)
			{
				position.y += -s * SPEED * dt;
			}

			else
			{
				position.y = (-s * position.x) + playerY;
			}
		}
	}

	else if (diferentialX < 0)
	{
		position.x -= SPEED * dt;

		if (diferentialY > 0)
		{
			float s = (float)diferentialY / diferentialX;

			if (app->fpsCapped == true)
			{
				position.y += -s * SPEED * dt;
			}

			else
			{
				position.y = (-s * position.x) + playerY;
			}

		}
		else if (diferentialY < 0)
		{
			int aux2;
			aux2 = diferentialY * -1;
			float s = (float)aux2 / diferentialX;

			if (app->fpsCapped == true)
			{
				position.y += s * SPEED * dt;
			}

			else
			{
				position.y = (s * position.x) + playerY;
			}
		}
	}

	col->SetPos(position.x, position.y);
	Draw();
}

void Particle::Draw()
{
	app->render->DrawTexture(sprite, position.x, position.y, &idle.GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
}

void Particle::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_WALL || c2->type == COLLIDER_ENEMY)
	{
		lives = 0;
	}

}