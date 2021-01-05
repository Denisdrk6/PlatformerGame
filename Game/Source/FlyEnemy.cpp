#include "FlyEnemy.h"
#include "Collisions.h"
#include "Textures.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "Player.h"

#define SPEED 100


FlyEnemy::FlyEnemy(iPoint pos) : Entity(EntityType::FLY_ENEMY)
{
	//load graphics
	sprite = app->tex->Load("Assets/enemies/flying_enemy.png");
	debug_tex = app->tex->Load("Assets/maps/pathRect.png");
	//Load animations
	idle.PushBack({ 0 , 51 , 46 , 45 });
	idle.PushBack({ 55, 51 , 46 , 45 });
	idle.PushBack({ 108, 51 , 46 , 45 });
	idle.PushBack({ 164, 51 , 46 , 45 });
	idle.PushBack({ 212, 51 , 46 , 45 });
	idle.PushBack({ 266, 51 , 46 , 45 });
	idle.speed = 0.05f;



	//add collider
	col = app->col->AddCollider({ position.x, position.y, 46,45 }, COLLIDER_ENEMY, app->entities);

	//save positions
	initialPosition = position = pos;

	//save lives
	lives = maxLives = 8;
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Load(pugi::xml_node& load)
{
	//lives = load.attribute("lives").as_int();
	position.x = load.child("position").attribute("x").as_int();
	position.y = load.child("position").attribute("y").as_int();

	initialPosition.x = load.child("initial_position").attribute("x").as_int();
	initialPosition.y = load.child("initial_position").attribute("y").as_int();

	vel.x = vel.y = 0;
}

void FlyEnemy::Save(pugi::xml_node& save) const
{
	//save.append_attribute("lives");
	//save.attribute("lives").set_value(lives);

	save.append_child("position");
	save.child("position").append_attribute("x");
	save.child("position").append_attribute("y");
	save.child("position").attribute("x").set_value(position.x);
	save.child("position").attribute("y").set_value(position.y);

	save.append_child("initial_position");
	save.child("initial_position").append_attribute("x");
	save.child("initial_position").append_attribute("y");
	save.child("initial_position").attribute("x").set_value(initialPosition.x);
	save.child("initial_position").attribute("y").set_value(initialPosition.y);
}

void FlyEnemy::Reset()
{
	position = initialPosition;
	vel.x = 0;
	vel.y = 0;
	lives = 5;
	if (col != nullptr)
	{
		app->col->DeleteCollider(col);
	}
	dead = false;
	falling = false;
	col = app->col->AddCollider({ position.x,position.y,32,32 }, COLLIDER_ENEMY, app->entities);
}

void FlyEnemy::Update(float dt)
{

	if (dead == false)
	{
		private_dt = dt;
		if (lives == 0)
		{
			dead = true;
			app->col->DeleteCollider(col);
			app->sceneManager->gameplay->FlyEnemies.Del(app->sceneManager->gameplay->FlyEnemies.At(app->sceneManager->gameplay->FlyEnemies.Find(this)));
			falling = true;
		}

		HandeInput();

		switch (CurrentState)
		{
		case FlyEnemy::NONE:
			//nothing
			break;
		case FlyEnemy::MOVE:
			position.x += vel.x * dt;//adjust values later * 80;
			position.y += vel.y * dt;//adjust values later
			break;
		default:
			break;
		}


		//FLip texture
		if (position.x > app->player->position.x) {
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (position.x < app->player->position.x) {
			flip = SDL_FLIP_NONE;
		}

		Draw();
		col->SetPos(position.x, position.y);
	}

	else if (dead == true) {
		if (falling == true) {
			vel.y = 100;
			position.y += vel.y * dt;
			if (col != nullptr) {
				col->SetPos(position.x, position.y);
			}
		}
		Current_animation = &death;
		app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
	}
}

void FlyEnemy::Draw()
{
	Current_animation = &idle;

	if (app->player->lifes > 0)
		app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);

	if (app->col->debug)
		blitPath();
}

void FlyEnemy::HandeInput()
{
	vel.x = vel.y = 0;
	CurrentState = NONE;

	if (ChasePlayer(app->player->position) == true)
	{
      		app->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->player->position.x, app->player->position.y), true);

		const DynArray<Path>* path = app->pathfinding->GetLastPath();

		const Path* path_dir = path->At(1);
		if (path_dir != nullptr)
		{

			switch (path_dir->dir)
			{
			case DIR_UP:
				vel.y = SPEED;
				CurrentState = MOVE;
				break;
			case DIR_DOWN:
				vel.y = -SPEED;
				CurrentState = MOVE;
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
}

bool FlyEnemy::ChasePlayer(fPoint player)
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

void FlyEnemy::OnCollision(Collider* c1, Collider* c2)
{
	//Collision with shot or player if necessary.
	//Decrease speed if touches player.
	//If touches floor vel.y = vel.x = 0
	//Less lives if touches player

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

	
}

void FlyEnemy::blitPath()
{
	const DynArray<Path>* path = app->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->PosX, path->At(i)->PosY);
		//if (debug_tex != nullptr)
		app->render->DrawTexture(debug_tex, pos.x, pos.y);
	}
}