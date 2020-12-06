#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Log.h"
#include "Player.h"

#include "Collisions.h"

Collisions::Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	//colliders matrix

	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEAD] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_END] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FINISH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CHECKPOINT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_HEART] = true;

	
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_WALL] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_DEAD] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_END] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_WIN] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_FINISH] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_COIN] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_HEART] = false;


	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_DEAD] = false;
	matrix[COLLIDER_WALL][COLLIDER_END] = false;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_FINISH] = false;
	matrix[COLLIDER_WALL][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_WALL][COLLIDER_COIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_HEART] = false;


	matrix[COLLIDER_DEAD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEAD][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_DEAD][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEAD][COLLIDER_DEAD] = false;
	matrix[COLLIDER_DEAD][COLLIDER_END] = false;
	matrix[COLLIDER_DEAD][COLLIDER_WIN] = false;
	matrix[COLLIDER_DEAD][COLLIDER_FINISH] = false;
	matrix[COLLIDER_DEAD][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_DEAD][COLLIDER_COIN] = false;
	matrix[COLLIDER_DEAD][COLLIDER_HEART] = false;


	matrix[COLLIDER_END][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_END][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_END][COLLIDER_WALL] = false;
	matrix[COLLIDER_END][COLLIDER_DEAD] = false;
	matrix[COLLIDER_END][COLLIDER_END] = false;
	matrix[COLLIDER_END][COLLIDER_WIN] = false;
	matrix[COLLIDER_END][COLLIDER_FINISH] = false;
	matrix[COLLIDER_END][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_END][COLLIDER_COIN] = false;
	matrix[COLLIDER_END][COLLIDER_HEART] = false;


	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEAD] = false;
	matrix[COLLIDER_WIN][COLLIDER_END] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_FINISH] = false;
	matrix[COLLIDER_WIN][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_WIN][COLLIDER_COIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_HEART] = false;


	matrix[COLLIDER_CHECKPOINT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_WALL] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_DEAD] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_END] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_WIN] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_FINISH] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_COIN] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_HEART] = false;

	
	
	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_COIN][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_COIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_COIN][COLLIDER_DEAD] = false;
	matrix[COLLIDER_COIN][COLLIDER_END] = false;
	matrix[COLLIDER_COIN][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_COIN][COLLIDER_COIN] = false;
	matrix[COLLIDER_COIN][COLLIDER_HEART] = false;


	matrix[COLLIDER_HEART][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_HEART][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_HEART][COLLIDER_WALL] = false;
	matrix[COLLIDER_HEART][COLLIDER_DEAD] = false;
	matrix[COLLIDER_HEART][COLLIDER_END] = false;
	matrix[COLLIDER_HEART][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_HEART][COLLIDER_COIN] = false;
	matrix[COLLIDER_HEART][COLLIDER_HEART] = false;

	matrix[COLLIDER_FINISH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FINISH][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FINISH][COLLIDER_WALL] = false;
	matrix[COLLIDER_FINISH][COLLIDER_DEAD] = false;
	matrix[COLLIDER_FINISH][COLLIDER_END] = false;
	matrix[COLLIDER_FINISH][COLLIDER_WIN] = false;
	matrix[COLLIDER_FINISH][COLLIDER_FINISH] = false;
	matrix[COLLIDER_FINISH][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_FINISH][COLLIDER_COIN] = false;
	matrix[COLLIDER_FINISH][COLLIDER_HEART] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DEAD] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_END] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_FINISH] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_COIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_HEART] = false;
	

	name.Create("map");
}

Collisions::~Collisions()
{}

bool Collisions::Awake(pugi::xml_node& config)
{
	return true;
}

bool Collisions::LoadColliders(pugi::xml_node& node)
{
	bool ret = true;
	COLLIDER_TYPE coltype;
	SString type;
	Module* call = nullptr;

	pugi::xml_node tileset = node.child("tileset");
	pugi::xml_node layer;
	pugi::xml_node typeNode;

	for (layer = node.child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		if ((SString)layer.attribute("name").as_string() == "Colisions")
		{
			int x = 0;
			int y = 0;
			pugi::xml_node tile;
			for (tile = layer.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
			{
				SDL_Rect rect;
				int id = tile.attribute("gid").as_int() - 1;
				if (y <= layer.attribute("height").as_int())
				{
					if (id != -1)
					{
						for (typeNode = tileset.child("tile"); typeNode; typeNode = typeNode.next_sibling("tile"))
						{
							if (id == typeNode.attribute("id").as_int())
							{
								type = typeNode.child("properties").child("property").attribute("value").as_string();

								if (type == "floor")
								{
									coltype = COLLIDER_FLOOR;
									LOG("Collider floor");
									call = app->map;
								}

								else if (type == "wall")
								{
									coltype = COLLIDER_WALL;
									LOG("Collider wall");
									call = app->map;
								}

								else if (type == "dead")
								{
									coltype = COLLIDER_DEAD;
									LOG("Collider dead");
								}

								else if (type == "win")
								{
									coltype = COLLIDER_WIN;
									LOG("Collider win");
								}

								else if (type == "finish")
								{
									coltype = COLLIDER_FINISH;
									LOG("Collider finish");
								}

								rect.x = app->map->MapToWorld(x, y).x;
								rect.y = app->map->MapToWorld(x, y).y;
								rect.w = tileset.attribute("tilewidth").as_int();
								rect.h = tileset.attribute("tileheight").as_int();

								app->map->data.colliders.Add(AddCollider(rect, coltype, call));
								LOG("%i x %i", rect.x, rect.y);
							}
						}
					}

				}
				else break;
				x++;
				if (x >= layer.attribute("width").as_int())
				{
					y++;
					x = 0;
				}
			}
		}
	}
	return true;
}

bool Collisions::PreUpdate()
{

	//Checks if player stops colliding with floor/wall
	playerFloorCol = 0;
	playerWallCol = 0;
	playerDeadCol = 0;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr && colliders[i]->toDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	//calculate collisi
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{

		//skip empty colliders
		if (colliders[i] == nullptr) {
			continue;
		}

		c1 = colliders[i];

		//avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {

			//skip empty colliders
			if (colliders[k] == nullptr) {
				continue;
			}

			c2 = colliders[k];

			if (c1->toDelete == false && c2->toDelete == false && c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
			{
				SDL_Rect nextPos = c1->rect;
				nextPos.y += app->player->speedY;
				if (c2->CheckCollision(nextPos) == true)
				{
					c1->callback->OnCollision(c1, c2);
				}
			}

			else if (c1->toDelete == false && c2->toDelete == false && c2->type == COLLIDER_TYPE::COLLIDER_PLAYER && c1->type == COLLIDER_TYPE::COLLIDER_FLOOR)
			{
				SDL_Rect nextPos = c2->rect;
				nextPos.y += app->player->speedY;
				if (c1->CheckCollision(nextPos) == true)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}

			if (c1->CheckCollision(c2->rect) == true)
			{

				if (matrix[c1->type][c2->type] && c1->toDelete == false && c1->callback) {
					c1->callback->OnCollision(c1, c2);
				}

				if (matrix[c2->type][c1->type] && c2->toDelete == false && c2->callback) {
					c2->callback->OnCollision(c2, c1);
				}

				//Checks if player stops colliding with floor/wall
				if ((c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR) || (c2->type == COLLIDER_TYPE::COLLIDER_PLAYER && c1->type == COLLIDER_TYPE::COLLIDER_FLOOR)) 
					playerFloorCol++;

				if ((c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WALL) || (c2->type == COLLIDER_TYPE::COLLIDER_PLAYER && c1->type == COLLIDER_TYPE::COLLIDER_WALL))
				{
					playerWallCol++;
					LOG("Player collides with wall");
				}

				if ((c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_DEAD) || (c2->type == COLLIDER_TYPE::COLLIDER_PLAYER && c1->type == COLLIDER_TYPE::COLLIDER_DEAD))
					playerDeadCol++;

			}
		}
	}

	//Checks if player is no longer touching a platform (walks to the edge and falls)
	if (playerFloorCol == 0 && app->player->firstFrame == false)
		app->player->groundCol = false;

	// First frame bool variable checks if we are on first frame so player doesn't jump when respawning
	if (app->player->firstFrame == true)
	{
		app->player->firstFrame = false;
	}

	if (playerWallCol == 0)
	{
		app->player->wallCol = false;
		app->player->downCol = false;
	}

	if (playerDeadCol == 0 && app->player->firstFrame == false)
	{
		app->player->lifeWait = 0;
		app->player->lifeTaken = false;
		app->player->waiting = false;
		/*app->player->textureHurt.loaded = false;
		app->player->textureHurt.alpha = 0;*/
	}

	return true;
}

bool Collisions::Update(float dt)
{
	DebugDraw();

	return true;
}

void Collisions::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
		app->map->collisionDraw = !app->map->collisionDraw;

	}

	if (debug == false)
	{
		return;
	}

	LOG("showing colliders");

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_PLAYER: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_FLOOR: // blue
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_WALL:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_END:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_CHECKPOINT:
			app->render->DrawRectangle(colliders[i]->rect, 100, 255, 155, alpha);
			break;
		case COLLIDER_COIN:
			app->render->DrawRectangle(colliders[i]->rect, 100, 100, 100, alpha);
			break;
		case COLLIDER_HEART:
			app->render->DrawRectangle(colliders[i]->rect, 200, 200, 200, alpha);
			break;
		case COLLIDER_WIN:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_FINISH:
			app->render->DrawRectangle(colliders[i]->rect, 80, 30, 20, alpha);
		case COLLIDER_DEAD:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_GODMODE:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		}
	}
}

bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if ((colliders[i] != nullptr) && (colliders[i]->type != COLLIDER_PLAYER))
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rectC, COLLIDER_TYPE typeC, Module* callbackC)
{
	Collider* ret = nullptr;
	for (uint j = 0; j < MAX_COLLIDERS; ++j)
	{
		if (colliders[j] == nullptr)
		{
			ret = colliders[j] = new Collider(rectC, typeC, callbackC);
			break;
		}
	}
	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	//return !(r.x + r.w<rect.x || r.x>rect.x + rect.w || r.y + r.h<rect.y || r.y>rect.y + rect.h);
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

//-----------------------------------------------------
void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}
void Collisions::DeleteCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		collider->toDelete = true;
	}
}
