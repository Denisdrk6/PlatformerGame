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
	update_ms_cycle = 1000.0f * (1 / (float)cycles);

	return ret;
}

bool Entities::Load(pugi::xml_node& load) {
	ListItem<Entity*>* ent;
	for (ent = entities.start; ent; ent = ent->next) {
		app->col->DeleteCollider(ent->data->col);
	}
	DestroyAll();




	pugi::xml_node node_fly;
	for (node_fly = load.child("Fly_Enemies").child("Fly_Enemy"); node_fly; node_fly = node_fly.next_sibling("FlyEnemy")) {
		iPoint pos;
		pos.x = pos.y = 0;

		FlyEnemy* ret = (FlyEnemy*)CreateEntity(Entity::EntityType::fly_enemy, pos);
		ret->Load(node_fly);
		FlyEnemies.Add(ret);
	}
	//____________________


	//Same with floor enemy
	/*pugi::xml_node node_floor;
	for (node_floor = load.child("Floor_Enemies").child("Floor_Enemy"); node_floor; node_floor = node_floor.next_sibling("Floor_Enemy")) {
		iPoint pos;
		pos.x = pos.y = 0;

		FloorEnemy* retF = (FloorEnemy*)CreateEntity(Entity::EntityType::floor_enemy, pos);
		ret->Load(node_floor);
		app->scene->FloorEnemies.Add(retF);
	}*/

	return true;
}

bool Entities::Save(pugi::xml_node& save) const {

	save.append_child("Fly_Enemies");
	save.child("Fly_Enemies").append_child("Fly_Enemy");
	pugi::xml_node fly_node;
	fly_node = save.child("Fly_Enemies").child("Fly_Enemy");
	ListItem<FlyEnemy*>* fly;
	for (fly = FlyEnemies.start; fly; fly = fly->next) {
		fly->data->Save(fly_node);
		if (fly->next != NULL) {
			save.child("Fly_Enemies").append_child("Fly_Enemy");
		}
		fly_node = fly_node.next_sibling("Fly_Enemy");
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

	UpdateEntities(dt, do_logic);
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
	case Entity::EntityType::fly_enemy:				ret = new FlyEnemy(pos);			break;
	case Entity::EntityType::floor_enemy:			ret = new FloorEnemy(pos);		break;
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

	ListItem<FlyEnemy*>* fly_enemy;
	for (fly_enemy = FlyEnemies.start; fly_enemy; fly_enemy = fly_enemy->next) {
		if (fly_enemy->data->col->rect.x == c1->rect.x && fly_enemy->data->col->rect.y == c1->rect.y && fly_enemy->data->col->rect.w == c1->rect.w && fly_enemy->data->col->rect.h == c1->rect.h) {
			fly_enemy->data->OnCollision(c1, c2);
		}
	}

}

void Entities::LoadFromObjectLayer(ObjectLayer* layer) {

	ListItem<ObjectData*>* obj;
	if (obj->data->name == "FlyEnemy") {
		iPoint pos;
		pos.x = obj->data->x;
		pos.y = obj->data->y;

		FlyEnemy* ret = (FlyEnemy*)CreateEntity(Entity::EntityType::fly_enemy, pos);
		FlyEnemies.Add(ret);
	}

	if (obj->data->name == "FloorEnemy") {
		iPoint pos;
		pos.x = obj->data->x;
		pos.y = obj->data->y;

		FloorEnemy* ret = (FloorEnemy*)CreateEntity(Entity::EntityType::floor_enemy, pos);
		FloorEnemies.Add(ret);
	}
}
