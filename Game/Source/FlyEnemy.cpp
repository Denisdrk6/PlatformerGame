#include "FlyEnemy.h"
#include "Collisions.h"
#include "Textures.h"
#include "Pathfinding.h"
#include "Scene.h"
#include "Player.h"

#define SPEED 100


FlyEnemy::FlyEnemy(iPoint pos) : Entity(EntityType::fly_enemy) {
	//load graphics
	sprite = app->tex->Load("Assets/enemies/enemies.png");

	//Load animations
	idle.PushBack({ 0 , 0 , 0 , 0 });
	idle.PushBack({ 0, 0 , 0 , 0 });
	idle.speed = 1.0f;



	//add collider
	col = app->col->AddCollider({ position.x, position.y, 28,28 }, COLLIDER_ENEMY, app->entities);

	//save positions
	initial_position = position = pos;

	//save lives
	lives = maxLives = 8;
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Load(pugi::xml_node& load) {
	//lives = load.attribute("lives").as_int();

	position.y = load.child("position").attribute("y").as_int() - 300;

	initial_position.x = load.child("initial_position").attribute("x").as_int();
	initial_position.y = load.child("initial_position").attribute("y").as_int();

	vel.x = vel.y = 0;
}

void FlyEnemy::Save(pugi::xml_node& save) const {
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
	save.child("initial_position").attribute("x").set_value(initial_position.x);
	save.child("initial_position").attribute("y").set_value(initial_position.y);
}

void FlyEnemy::Reset() {
	position = initial_position;
	vel.x = 0;
	vel.y = 0;
	lives = maxLives;
	if (col != nullptr) {
		app->col->DeleteCollider(col);
	}
	dead = false;
	falling = false;
	col = app->col->AddCollider({ position.x,position.y,32,32 }, COLLIDER_ENEMY, app->entities);
}

void FlyEnemy::Update(float dt) {

	if (dead == false) {
		private_dt = dt;
		if (lives == 0) {
			dead = true;
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
		app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1.0f, NULL, NULL, NULL);
	}
}

void FlyEnemy::Draw() {
	Current_animation = &idle;


	app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1.0f, NULL, NULL, NULL);
}

void FlyEnemy::HandeInput() {
	vel.x = vel.y = 0;
	CurrentState = NONE;

	if (ChasePlayer(app->player->position) == true) {
		app->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->player->position.x, app->player->position.y), true);

		const DynArray<Path>* path = app->pathfinding->GetLastPath();

		const Path* path_dir = path->At(1);
		if (path_dir != nullptr) {

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

bool FlyEnemy::ChasePlayer(fPoint player) {
	player.x = (int)player.x;
	player.y = (int)player.y;

	if (player.x + 16 > position.x - 500 && player.x + 16 < position.x + 500) {
		if (player.y > position.y - 200 && player.y < position.y + 300) {
			return true;
		}
	}

	return false;
}

void FlyEnemy::OnCollision(Collider* c1, Collider* c2) {
	//Collision with shot or player if necessary.
	//Decrease speed if touches player.
	//If touches floor vel.y = vel.x = 0
	//Less lives if touches player
}