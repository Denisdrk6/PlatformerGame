
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}
bool Map::LoadObjLayer(pugi::xml_node & node, ObjectLayer * layer) 
 {
	bool ret = true;

	layer->name = node.attribute("name").as_string();

	pugi::xml_node object;
	for (object = node.child("object"); object; object = object.next_sibling("object")) {
		ObjectData* obj_data = new ObjectData();

		obj_data->name = object.attribute("name").as_string();
		obj_data->x = object.attribute("x").as_int();
		obj_data->y = object.attribute("y").as_int();
		obj_data->w = object.attribute("width").as_int();
		obj_data->h = object.attribute("height").as_int();

		layer->list.Add(obj_data);
	}

	return ret;
}
//bool Map::Start()
//{
//	data.tilesets.start->data->texture = 
//}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layer = data.layers.start;
	ListItem<TileSetInfo*>* tileset = data.tilesets.start;
	//int i = 0;
	while (layer != NULL && tileset != NULL)
	{
		if (collisionDraw == false)
		{
			if (layer->data->name != "Colisions" && layer->data->name != "Navigation")
			{
				for (int y = 0; y < data.height; ++y)
				{
					for (int x = 0; x < data.width; ++x)
					{

						/*if (layer->data->tilesetNum != i)
						{
							tileset = tileset->next;
							i++;
						}*/

						int tileId = layer->data->Get(x, y);
						if (tileId > 0)
						{
							app->render->DrawTexture(tileset->data->texture, MapToWorld(x, y).x, MapToWorld(x, y).y, &tileset->data->GetTileRect(tileId), 1);
						}
					}
				}
			}
		}

		else
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					/*if (layer->data->tilesetNum != i)
					{
						tileset = tileset->next;
						i++;
					}*/

					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						app->render->DrawTexture(tileset->data->texture, MapToWorld(x, y).x, MapToWorld(x, y).y, &tileset->data->GetTileRect(tileId), 1);
					}
				}
			}
		}
		layer = layer->next;
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{

		ret.x = x; ret.y = y;
	}

	return ret;
}


TileSetInfo* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSetInfo*>* item = data.tilesets.start;
	TileSetInfo* set = item->data;

	while (item)
	{
		if (id < item->data->firstGid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSetInfo::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	
	int relativeId = id - firstGid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSetInfo*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		//app->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Remove colliders
	ListItem<Collider*>* item3;
	item3 = data.colliders.start;

	while (item3 != NULL)
	{
		app->col->CleanUp();
		//app->col->DeleteCollider(item3->data);
		item3 = item3->next;
	}
	data.colliders.Clear();

	data.obj_layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		LOG("Loading map data...");
		ret = LoadMapData(mapFile.child("map"));
	}

	int i = 0  ;
	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	for (pugi::xml_node tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
	{
		if (ret == true)
		{
			TileSetInfo* info = new TileSetInfo;

			LOG("Loading tilesets data...");

			ret = LoadTileset(tileset, info);
			data.tilesets.Add(info);
		}
		pugi::xml_node layer;
		for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* set2 = new MapLayer();

			if (ret == true) ret = LoadLayer(layer, set2);
			data.layers.Add(set2);
			
		}

		pugi::xml_node object;
		for (object = mapFile.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
		{
			if (object.child("properties").child("property").attribute("value").as_int() == i)
			{
				ObjectLayer* set3 = new ObjectLayer;

				if (ret == true) ret = LoadObjLayer(object, set3 );

				data.obj_layers.Add(set3);
			}
		}

		if (ret == true)
		{
			LOG("Succesfully parsed XML file: %s", filename);
			LOG("width: %d height: %d", data.width, data.height);
			LOG("tile_width: %d tile_height %d", data.tileWidth, data.tileHeight);

			for (int i = 0; i < data.tilesets.Count(); i++)
			{
				LOG("Tileset %d ----", i + 1);
				LOG("name: %s firstgid: %d", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstGid);
				LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->imageWidth, data.tilesets.At(i)->data->imageHeight);
				LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);
			}
		}

		//Load colliders info 
		if (ret == true) {
			ret = app->col->LoadColliders(mapFile.child("map"));
		}

		if (ret == true)
		{
			LOG("Successfully parsed map XML file: %s", filename);
			LOG("width: %d height: %d", data.width, data.height);
			LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);
			LOG("");

			ListItem<TileSetInfo*>* item = data.tilesets.start;
			while (item != NULL)
			{
				TileSetInfo* s = item->data;
				LOG("Tileset ----");
				//LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
				LOG("tile width: %d tile height: %d", s->tileWidth, s->tileHeight);
				LOG("spacing: %d margin: %d", s->spacing, s->margin);

				item = item->next;
			}

			ListItem<MapLayer*>* item_layer = data.layers.start;
			while (item_layer != NULL)
			{
				MapLayer* l = item_layer->data;
				LOG("Layer ----");
				LOG("name: %s", l->name.GetString());
				LOG("tile width: %d tile height: %d", l->width, l->height);
				LOG("");
				item_layer = item_layer->next;
			}
		}
		i++;
	}
	mapLoaded = ret;
	return ret;
}

bool Map::LoadMapData(pugi::xml_node mapData)
{
    bool ret = true;

    if (mapData == NULL) ret = false;

    data.orientation = mapData.attribute("orientation").as_string();
	data.renderOrder = mapData.attribute("renderorder").as_string();
	data.width = mapData.attribute("width").as_int();
	data.height = mapData.attribute("height").as_int();
	data.tileWidth = mapData.attribute("tilewidth").as_int();
	data.tileHeight = mapData.attribute("tileheight").as_int();
	data.nextObjectId = mapData.attribute("nextobjectid").as_int();
	data.type = MapTypes::MAPTYPE_ORTHOGONAL;

    return ret;
}

bool Map::LoadTileset(pugi::xml_node tileset, TileSetInfo* info)
{
    bool ret = true;

    pugi::xml_node imageNode = tileset.child("image");

    if (imageNode == NULL) ret = false;

    info->name = tileset.attribute("name").as_string();
    info->firstGid = tileset.attribute("firstgid").as_int();
    info->tileWidth = tileset.attribute("tilewidth").as_int();
    info->tileHeight = tileset.attribute("tileheight").as_int();
    info->margin = tileset.attribute("margin").as_int();
    info->spacing = tileset.attribute("spacing").as_int();

    info->source = imageNode.attribute("source").as_string();
    info->imageWidth = imageNode.attribute("width").as_int();
    info->imageHeight = imageNode.attribute("height").as_int();
	info->numTilesWidth = info->imageWidth / info->tileWidth;
	info->numTilesHeight = info->imageHeight / info->tileHeight;

    SString tmp("%s%s", folder.GetString(), info->source.GetString());
    info->texture = app->tex->Load(tmp.GetString());

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->name = node.attribute("name").as_string();
	layer->data = new unsigned int[layer->width * layer->height * sizeof(unsigned int)];
	memset(layer->data, 0, layer->width * layer->height * sizeof(unsigned int));
	int i = 0;
	for (pugi::xml_node tile =node.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = (unsigned int)tile.attribute("gid").as_int();
		i++;
	}

	return ret;
}
void Map::OnCollision(Collider* c1, Collider* c2) {
	if (c1->type == COLLIDER_FLOOR && c2->type == COLLIDER_PLAYER) {
		if (c2->rect.y < c1->rect.y) {
			if (c1->rect.y < c2->rect.y + c2->rect.h - 3) {
				if (c1->rect.x < c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w / 2 > c2->rect.x) {
					//app->player->position.x = c1->rect.x - c2->rect.w;
				}
				if (c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w / 2 < c2->rect.x) {
					//app->player->position.x = c1->rect.x + c1->rect.w;
				}
			}
		}
		else {
			if (c1->rect.x < c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w / 2 > c2->rect.x) {
				//app->player->position.x = c1->rect.x - c2->rect.w;
			}
			if (c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w / 2 < c2->rect.x) {
				//app->player->position.x = c1->rect.x + c1->rect.w;
			}
		}
	}
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->name != "Navigation")
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSetInfo* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstGid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}