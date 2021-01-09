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
	if (position.x >= app->win->width*app->map->data.tileWidth|| position.y >= app->win->height * app->map->data.tileHeight)
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
			int aux1_bis;
			aux1_bis = diferentialY * -1;
			float s = (float)aux1_bis / diferentialX;

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
			int aux1_bis;
			aux1_bis = diferentialY * -1;
			float s = (float)aux1_bis / diferentialX;

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

	float angle = 0.0f;

	if (diferentialX > 0)
	{

		if (diferentialY > 0)
		{
			float s = (float)diferentialY / diferentialX;
			angle = s * 40;
		}

		else if (diferentialY < 0)
		{
			int aux1_bis;
			aux1_bis = diferentialY * -1;
			float s = (float)aux1_bis / diferentialX;
			angle = s * -40;
		}
	}

	else if (diferentialX < 0)
	{

		if (diferentialY > 0)
		{
			float s = (float)diferentialY / diferentialX;
			angle = (s * 40) + 180;
		}

		else if (diferentialY < 0)
		{
			int aux1_bis;
			aux1_bis = diferentialY * -1;
			float s = (float)aux1_bis / diferentialX;
			angle = (s * -40) + 180;
		}
	}

	//app->render->DrawTexture(sprite, position.x, position.y, &idle.GetCurrentFrame(), 1.0f, angle);
	app->render->DrawTexture(sprite, position.x, position.y, &idle.GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
}

void Particle::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_WALL || c2->type == COLLIDER_ENEMY)
	{
		lives = 0;
	}

}