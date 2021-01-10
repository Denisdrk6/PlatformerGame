#include "Particles.h"
#include "Textures.h"
#include "Scene.h"
#include "Player.h"
#include "Collisions.h"
#include "App.h"
#include "Pathfinding.h"
#include "Window.h"

#define SPEED 10

Particle::Particle(iPoint pos, int x, int y) : Entity(EntityType::PARTICLE)
{
	position = pos;

	sprite = app->player->shot;
	
	idle.PushBack({ 57,57,36,36 });
	idle.PushBack({ 57,57,36,36 });
	idle.speed = 7.0f;

	iPoint vOrigin = { x - pos.x, 0 };
	iPoint vDestination = { x - pos.x, y - pos.y };

	float pointProduct = (vOrigin.x * vDestination.x) + (vOrigin.y * vDestination.y);

	float originMagnitude = sqrt(pow(vOrigin.x, 2) + pow(vOrigin.y, 2));
	float destinationMagnitude = sqrt(pow(vDestination.x, 2) + pow(vDestination.y, 2));

	float angle = acos(pointProduct / (originMagnitude * destinationMagnitude));

	if (x > pos.x)
	{
		speedX = SPEED * cos(angle);
	}

	else speedX = -SPEED * cos(angle);

	if (y > pos.y)
	{
		speedY = SPEED * sin(angle);
	}

	else speedY = SPEED * sin(-angle);

	col = app->col->AddCollider({ position.x,position.x,30,30 }, COLLIDER_SHOT, app->entities);

	lives = 1;

	app->pathfinding->lastPath.Clear();
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

	position.x += speedX;
	position.y += speedY;

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