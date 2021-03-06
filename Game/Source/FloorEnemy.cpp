#include "FloorEnemy.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "SceneManager.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "App.h"

#define SPEED 100


FloorEnemy::FloorEnemy(iPoint pos) : Entity(EntityType::FLOOR_ENEMY)
{
	//Load Sprite
	sprite = app->tex->Load("Assets/Enemies/floor_enemy.png");

	//Load Animations
	rIdleAnim.PushBack({ 1*2, 2*2, 17*2, 26*2 });
	rIdleAnim.PushBack({ 33*2, 1*2, 17*2, 27*2 });
	rIdleAnim.PushBack({ 64*2, 1*2, 18*2, 27*2 });
	rIdleAnim.PushBack({ 95*2, 2*2, 20*2, 26*2 });
	rIdleAnim.speed = 0.02f;

	lIdleAnim.PushBack({ 98*2, 30*2, 17*2, 26*2 });
	lIdleAnim.PushBack({ 66 * 2, 29 * 2, 17 * 2, 27 * 2 });
	lIdleAnim.PushBack({ 34 * 2, 29 * 2, 18 * 2, 27 * 2 });
	lIdleAnim.PushBack({ 1 * 2, 30 * 2, 20 * 2, 26 * 2 });

	lIdleAnim.speed = 0.02f;

	rWalkAnim.PushBack({ 1 * 2, 59 * 2, 17 * 2, 25 * 2 });
	rWalkAnim.PushBack({ 33 * 2, 58 * 2, 17 * 2, 26 * 2 });
	rWalkAnim.PushBack({ 65 * 2, 57 * 2, 17 * 2, 27 * 2 });
	rWalkAnim.PushBack({ 96 * 2, 59 * 2, 18 * 2, 25 * 2 });
	rWalkAnim.PushBack({ 129 * 2, 58 * 2, 17 * 2, 26 * 2 });
	rWalkAnim.PushBack({ 161 * 2, 57 * 2, 17 * 2, 27 * 2 });
	rWalkAnim.speed = 0.1f;

	lWalkAnim.PushBack({ 322, 174, 34, 50 });
	lWalkAnim.PushBack({ 258, 172, 34, 52 });
	lWalkAnim.PushBack({ 194, 170, 34, 54 });
	lWalkAnim.PushBack({ 130, 174, 36, 50 });
	lWalkAnim.PushBack({ 66, 172, 34, 52 });
	lWalkAnim.PushBack({ 2, 170, 34, 54 });
	lWalkAnim.speed = 0.1f;

	rJumpAnim.PushBack({ 2, 234, 34, 52 });
	rJumpAnim.PushBack({ 62, 238, 40, 48 });
	rJumpAnim.PushBack({ 124, 236, 44, 48 });
	rJumpAnim.PushBack({ 194, 226, 34, 60 });
	rJumpAnim.PushBack({ 256, 226, 36, 54 });
	rJumpAnim.PushBack({ 318, 230, 40, 54 });
	rJumpAnim.PushBack({ 380, 238, 44, 48 });
	rJumpAnim.PushBack({ 448, 234, 36, 52 });
	rJumpAnim.speed = 0.025f;

	lJumpAnim.PushBack({ 450, 296, 34, 52 });
	lJumpAnim.PushBack({ 384, 300, 40, 48 });
	lJumpAnim.PushBack({ 318, 300, 44, 48 });
	lJumpAnim.PushBack({ 258, 288, 34, 60 });
	lJumpAnim.PushBack({ 194, 288, 36, 54 });
	lJumpAnim.PushBack({ 128, 292, 40, 54 });
	lJumpAnim.PushBack({ 62, 300, 44, 48 });
	lJumpAnim.PushBack({ 2, 296, 36, 52 });
	lJumpAnim.speed = 0.025f;

	col = app->col->AddCollider({ position.x,position.y,36,55 }, COLLIDER_ENEMY, app->entities);
	CurrentAnimation = &rIdleAnim;
	//Load position and save it
	initialPosition = position = pos;

	lives = 1;
}

FloorEnemy::~FloorEnemy() {}

/*void FloorEnemy::PreUpdate(float dt) {
	//private_dt = dt;
}*/

void FloorEnemy::Load(pugi::xml_node& load)
{
	//lives = load.attribute("lives").as_int();

	position.x = load.child("position").attribute("x").as_int();
	position.y = load.child("position").attribute("y").as_int();

	initialPosition.x = load.child("initial_position").attribute("x").as_int();
	initialPosition.y = load.child("initial_position").attribute("y").as_int();

	vel.x = vel.y = 0;

	// in case anything goes wrong loading a dead enemy, transport it to -3000, -3000
	if (position.x == 0 && position.y == 0)
	{
		position.x = -3000;
		position.y = -3000;
	}
}

void FloorEnemy::Save(pugi::xml_node& save) const
{
	//save.append_attribute("lives");
	//save.attribute("lives").set_value(lives);
	if (lives != 0)
	{
		save.append_child("position");
		save.child("position").append_attribute("x");
		save.child("position").append_attribute("y");
		save.child("position").attribute("x").set_value(position.x);
		save.child("position").attribute("y").set_value(position.y);
	}
		
	save.append_child("initial_position");
	save.child("initial_position").append_attribute("x");
	save.child("initial_position").append_attribute("y");
	save.child("initial_position").attribute("x").set_value(initialPosition.x);
	save.child("initial_position").attribute("y").set_value(initialPosition.y);
	
}

void FloorEnemy::Reset()
{
	position = initialPosition;
	vel.x = 0;
	vel.y = 0;
	lives = 1;
	if (col != nullptr) {
		app->col->DeleteCollider(col);
	}
	col = app->col->AddCollider({ position.x,position.y,36,55 }, COLLIDER_ENEMY, app->entities);
	dead = false;
}

void FloorEnemy::Update(float dt)
{

	if (dead == false)
	{
		if (lives == 0)
		{
			dead = true;
			app->audio->PlayFx(app->entities->deadFx);
			app->sceneManager->gameplay->points +=75;
			app->col->DeleteCollider(col);
			col = nullptr;
			//app->sceneManager->gameplay->FloorEnemies.Del(app->sceneManager->gameplay->FloorEnemies.At(app->sceneManager->gameplay->FloorEnemies.Find(this)));
			app->pathfinding->lastPath.Clear();
		}

		else
		{
			HandeInput();

			switch (CurrentState)
			{
			case FloorEnemy::NONE:
				//nothing
				break;
			case FloorEnemy::MOVE:
				position.x += vel.x * dt;
				break;
			default:
				break;
			}

			position.y += vel.y * dt;

			if (position.x > app->player->position.x)
			{
				//flip = SDL_FLIP_HORIZONTAL;
				CurrentAnimation = &lIdleAnim;

				if (vel.x != 0)
				{
					CurrentAnimation = &lWalkAnim;
				}

			}
			else if (position.x < app->player->position.x)
			{
				CurrentAnimation = &rIdleAnim;

				if (vel.x != 0)
				{
					CurrentAnimation = &rWalkAnim;
				}
			}

			if (falling == true)
			{
				if (position.x < app->player->position.x)
				{
					CurrentAnimation = &rJumpAnim;
				}
				else if (position.x > app->player->position.x)
				{
					CurrentAnimation = &lJumpAnim;
				}
			}


			Draw();
			col->SetPos(position.x, position.y);
			falling = true;
		}
	}
	else if (dead == true)
	{
		CurrentAnimation = &death;
		app->render->DrawTexture(sprite, position.x, position.y, &CurrentAnimation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
	}
}

bool FloorEnemy::ChasePlayer(iPoint player)
{
	player.x = (int)player.x;
	player.y = (int)player.y;

	if (player.x + 16 > position.x - 500 && player.x + 16 < position.x + 500)
	{
		if (player.y > position.y - 200 && player.y < position.y + 300)
		{
			return true;
		}
	}

	return false;
}

void FloorEnemy::HandeInput()
{
	vel.x = 0;

	CurrentState = NONE;

	if (ChasePlayer(app->player->position) == true)
	{
		app->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->player->position.x, app->player->position.y + app->map->data.tileHeight + app->map->data.tileHeight / 2), true);

		const DynArray<Path>* path = app->pathfinding->GetLastPath();

		const Path* path_dir = path->At(1);
		if (path_dir != nullptr)
		{

			switch (path_dir->dir)
			{
			case DIR_UP:
				//vel.y = SPEED;
				//CurrentState = MOVE;
				break;
			case DIR_DOWN:
				//vel.y = -SPEED;
				//CurrentState = MOVE;
				break;
			case DIR_LEFT:
				vel.x = -SPEED;
				CurrentState = MOVE;
				break;
			case DIR_RIGHT:
				vel.x = SPEED;
				CurrentState = MOVE;
				break;
			}
		}
	}

	if (falling == true)
	{
		vel.y += 2;
	}
}

void FloorEnemy::Draw()
{
	if(app->player->lifes > 0)
		app->render->DrawTexture(sprite, position.x, position.y, &CurrentAnimation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
}

void FloorEnemy::OnCollision(Collider* c1, Collider* c2)
{

	if (c1->type == COLLIDER_TYPE::COLLIDER_ENEMY && c2->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		int offset = -2 - (120 / (1 / app->dt));
		if (offset < -7) offset = -7;
		SDL_Rect nextCollision = { c1->rect.x, c1->rect.y + vel.y, c1->rect.w, c1->rect.h };

		if ((app->player->CheckTunneling(nextCollision, c1->rect) == true || c1->rect.y - (c2->rect.h + c2->rect.y) >= offset) && app->player->speedY >= app->player->maxNegativeSpeedY)
		{
			lives = 0;
		}
	}

	if (c1->type == COLLIDER_TYPE::COLLIDER_ENEMY && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
	{
		vel.y = 0;
		falling = false;
	}

	if (c1->type == COLLIDER_TYPE::COLLIDER_ENEMY && c2->type == COLLIDER_TYPE::COLLIDER_SHOT)
	{
		lives = 0;
	}
}