#include "FloorEnemy.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Scene.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "App.h"

#define SPEED 100


FloorEnemy::FloorEnemy(iPoint pos) : Entity(EntityType::FLOOR_ENEMY) {
	//Load Sprite
	sprite = app->tex->Load("Assets/enemies/ground_enemy.png");

	//Load Animations

	idle.PushBack({ 0 , 51 , 46 , 45 });;
	idle.PushBack({ 55, 51 , 46 , 45 });;
	idle.speed = 0.05f;

	col = app->col->AddCollider({ position.x,position.y,36,36 }, COLLIDER_ENEMY, app->entities);

	//Load position and save it
	initialPosition = position = pos;

	lives = 5;
}

FloorEnemy::~FloorEnemy() {}

/*void FloorEnemy::PreUpdate(float dt) {
	//private_dt = dt;
}*/

void FloorEnemy::Load(pugi::xml_node& load) {
	//lives = load.attribute("lives").as_int();

	position.x = load.child("position").attribute("x").as_int();
	position.y = load.child("position").attribute("y").as_int() - 300;

	initialPosition.x = load.child("initial_position").attribute("x").as_int();
	initialPosition.y = load.child("initial_position").attribute("y").as_int();

	vel.x = vel.y = 0;
}

void FloorEnemy::Save(pugi::xml_node& save) const {
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

void FloorEnemy::Reset() {
	position = initialPosition;
	vel.x = 0;
	vel.y = 0;
	lives = 5;
	if (col != nullptr) {
		app->col->DeleteCollider(col);
	}
	col = app->col->AddCollider({ position.x,position.y,64,64 }, COLLIDER_ENEMY, app->entities);
	dead = false;
}

void FloorEnemy::Update(float dt) {

	if (dead == false) {
		if (lives == 0) {
			dead = true;
			app->col->DeleteCollider(col);
		}

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

		if (position.x > app->player->position.x) {
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (position.x < app->player->position.x) {
			flip = SDL_FLIP_NONE;
		}

		Draw();
		col->SetPos(position.x, position.y);
		falling = true;
	}
	else if (dead == true) {
		Current_animation = &death;
		app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
	}
}

bool FloorEnemy::ChasePlayer(fPoint player) {
	player.x = (int)player.x;
	player.y = (int)player.y;

	if (player.x + 16 > position.x - 500 && player.x + 16 < position.x + 500) {
		//if (player.y > position.y - 200 && player.y < position.y + 300) {
			return true;
		}
	//}

	return false;
}

void FloorEnemy::HandeInput() {
	vel.x = 0;

	CurrentState = NONE;

	if (ChasePlayer(app->player->position) == true) {
		app->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->player->position.x, app->player->position.y + app->map->data.tileHeight + app->map->data.tileHeight / 2), false);

		const DynArray<Path>* path = app->pathfinding->GetLastPath();

		const Path* path_dir = path->At(1);
		if (path_dir != nullptr) {

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

	if (falling == true) {
		vel.y += 2;
	}
}

void FloorEnemy::Draw() {

	Current_animation = &idle;

	app->render->DrawTexture(sprite, position.x, position.y, &Current_animation->GetCurrentFrame(), 1, 1.0f, NULL, NULL, NULL);
}

void FloorEnemy::OnCollision(Collider* c1, Collider* c2) {

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
		int offset = -2 - (120 / (1 / app->dt));
		if (offset < -7) offset = -7;
		SDL_Rect nextCollision = { c1->rect.x, c1->rect.y + vel.y, c1->rect.w, c1->rect.h };
		vel.y = 0;
	}
}