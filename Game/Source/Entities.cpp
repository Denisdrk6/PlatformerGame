#include "App.h"
#include "Entities.h"
#include "Player.h"
#include "FlyEnemy.h"
#include "FloorEnemy.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"
#include "Audio.h"
#include "Fonts.h"

Entities::Entities() {
	name.Create("entities");
}

bool Entities::Awake(pugi::xml_node& config) {
	bool ret = true;

	int cycles;

	cycles = config.attribute("cycles_in_second").as_int();
	updateMsCycle = 1000.0f * (1 / (float)cycles);

	return ret;
}

bool Entities::Load(pugi::xml_node& load) {
	ListItem<Entity*>* ent;
	for (ent = entities.start; ent; ent = ent->next) {
		app->col->DeleteCollider(ent->data->col);
	}
	DestroyAll();




	pugi::xml_node nodeFly;
	for (nodeFly = load.child("Fly_Enemies").child("Fly_Enemy"); nodeFly; nodeFly = nodeFly.next_sibling("FlyEnemy")) {
		iPoint pos;
		pos.x = pos.y = 0;

		FlyEnemy* ret = (FlyEnemy*)CreateEntity(Entity::EntityType::FLY_ENEMY, pos);
		ret->Load(nodeFly);
		app->scene->FlyEnemies.Add(ret);
	}
	//____________________


	//Same with floor enemy
	pugi::xml_node nodeFloor;
	for (nodeFloor = load.child("Floor_Enemies").child("Floor_Enemy"); nodeFloor; nodeFloor = nodeFloor.next_sibling("Floor_Enemy")) {
		iPoint pos;
		pos.x = pos.y = 0;

		FloorEnemy* retF = (FloorEnemy*)CreateEntity(Entity::EntityType::FLOOR_ENEMY, pos);
		retF->Load(nodeFloor);
		app->scene->FloorEnemies.Add(retF);
	}

	return true;
}

bool Entities::Save(pugi::xml_node& save) const {

	save.append_child("Fly_Enemies");
	save.child("Fly_Enemies").append_child("Fly_Enemy");
	pugi::xml_node flyNode;
	flyNode = save.child("Fly_Enemies").child("Fly_Enemy");
	ListItem<FlyEnemy*>* fly;
	for (fly = app->scene->FlyEnemies.start; fly; fly = fly->next) {
		fly->data->Save(flyNode);
		if (fly->next != NULL) {
			save.child("Fly_Enemies").append_child("Fly_Enemy");
		}
		flyNode = flyNode.next_sibling("Fly_Enemy");
	}

	return true;
}

bool Entities::PreUpdate(float dt) {
	ListItem<Entity*>* AuxEntity = entities.start;

	for (; AuxEntity != NULL; AuxEntity = AuxEntity->next) {
		AuxEntity->data->PreUpdate(dt);
	}

	return true;
}

bool Entities::Update(float dt) {
	/*
	accumulated_time += dt;

	if (accumulated_time >= update_ms_cycle) {
		do_logic = true;
	}*/

	UpdateEntities(dt, doLogic);
	/*
		if (do_logic == true) {
			accumulated_time = 0.0f;
			do_logic = false;
		}*/

	return true;
}

void Entities::UpdateEntities(float dt, bool do_logic) {
	ListItem<Entity*>* AuxEntity = entities.start;

	for (; AuxEntity != NULL; AuxEntity = AuxEntity->next) {
		AuxEntity->data->Update(dt);
	}
}

Entity* Entities::CreateEntity(Entity::EntityType type, iPoint pos, int dest_X, int dest_Y) {
	Entity* ret = nullptr;
	switch (type) {
	case Entity::EntityType::FLY_ENEMY:				ret = new FlyEnemy(pos);			break;
	case Entity::EntityType::FLOOR_ENEMY:			ret = new FloorEnemy(pos);		break;
		//case Entity::EntityType::particle:				ret = new Particle(pos, dest_X, dest_Y); break;
	}

	if (ret != nullptr) {
		entities.Add(ret);
	}

	return ret;
}

void Entities::ResetEntities() {
	ListItem<Entity*>* AuxEntity = entities.start;

	for (; AuxEntity != NULL; AuxEntity = AuxEntity->next) {
		AuxEntity->data->Reset();
	}
}

void Entities::DestroyEntity(Entity* entity) {
	int entity_pos;
	ListItem<Entity*>* AuxEntity;

	entity_pos = entities.Find(entity);
	if (entity_pos != -1) {
		AuxEntity = entities.At(entity_pos);
		entities.Del(AuxEntity);
	}
}

void Entities::DestroyAll() {
	entities.Clear();
}

void Entities::OnCollision(Collider* c1, Collider* c2) {

	if (c1->type == COLLIDER_PLAYER) {
		app->player->OnCollision(c1, c2);
	}

	ListItem<FlyEnemy*>* flyEnemy;
	for (flyEnemy = app->scene->FlyEnemies.start; flyEnemy; flyEnemy = flyEnemy->next) {
		if (flyEnemy->data->col->rect.x == c1->rect.x && flyEnemy->data->col->rect.y == c1->rect.y && flyEnemy->data->col->rect.w == c1->rect.w && flyEnemy->data->col->rect.h == c1->rect.h) {
			flyEnemy->data->OnCollision(c1, c2);
		}
	}

	ListItem<FloorEnemy*>* floorEnemy;
	for (floorEnemy = app->scene->FloorEnemies.start; floorEnemy; floorEnemy = floorEnemy->next) {
		if (floorEnemy->data->col->rect.x == c1->rect.x && floorEnemy->data->col->rect.y == c1->rect.y && floorEnemy->data->col->rect.w == c1->rect.w && floorEnemy->data->col->rect.h == c1->rect.h) {
			floorEnemy->data->OnCollision(c1, c2);
		}
	}

}

void Entities::LoadFromObjectLayer(ObjectLayer* layer) {

	ListItem<ObjectData*>* obj;
	for (obj = layer->list.start; obj; obj = obj->next)
	{
		if (obj->data->name == "FlyEnemy") {
			iPoint pos;
			pos.x = obj->data->x;
			pos.y = obj->data->y;

			FlyEnemy* ret = (FlyEnemy*)CreateEntity(Entity::EntityType::FLY_ENEMY, pos);
			app->scene->FlyEnemies.Add(ret);
		}

		if (obj->data->name == "FloorEnemy") {
			iPoint pos;
			pos.x = obj->data->x;
			pos.y = obj->data->y;

			FloorEnemy* ret = (FloorEnemy*)CreateEntity(Entity::EntityType::FLOOR_ENEMY, pos);
			app->scene->FloorEnemies.Add(ret);
		}
	}
}