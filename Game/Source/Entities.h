#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "Module.h"
#include "Point.h"
#include "List.h"
#include "Render.h"
#include "Audio.h"

struct Collider;
struct ObjectLayer;


class Entity
{
public:
	enum class EntityType {
		PLAYER = 0,
		FLOOR_ENEMY,
		FLY_ENEMY,
		particle,
		UNKNOWN
	};

	iPoint position;
	EntityType type;
	SDL_Texture* sprite;
	int lives;

	Collider* col;

	//-------------------
	Entity::Entity(EntityType type) : type(type)
	{}

	virtual void PreUpdate(float dt) {}
	virtual void Update(float dt) {}
	virtual void Draw() {}
	virtual void HandeInput() {}
	virtual void Reset() {}
	virtual void Load(pugi::xml_node&) {}
	virtual void Save(pugi::xml_node&) const {}
	virtual void OnCollision(Collider* c1, Collider* c2) {}
};

class Entities : public Module
{
public:
	Entities();
	~Entities() {}

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool Awake(pugi::xml_node& config);
	void ResetEntities();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadFromObjectLayer(ObjectLayer* layer);
	Entity* CreateEntity(Entity::EntityType type, iPoint pos, int dest_X = NULL, int dest_Y = NULL);
	void DestroyEntity(Entity* entity);
	void DestroyAll();

	int deadFx = 0;

private:

	void UpdateEntities(float dt, bool do_logic);

	List<Entity*> entities;

	float accumulatedTime;
	float updateMsCycle;

	bool doLogic = false;
};

#endif