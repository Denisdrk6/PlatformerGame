
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

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

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layer = data.layers.start;
	ListItem<TileSetInfo*>* tileset = data.tilesets.start;
	int i = 0;
	while (layer != NULL && tileset != NULL)
	{
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					// L04: TODO 9: Complete the draw function
					app->render->DrawTexture(tileset->data->texture, MapToWorld(x, y).x, MapToWorld(x, y).y, &tileset->data->GetTileRect(tileId));
				}
			}
		}
		i++;
		layer = layer->next;
		tileset = tileset->next;
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

// Get relative Tile rectangle
SDL_Rect TileSetInfo::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	
	// L04: TODO 7: Get relative Tile rectangle
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
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

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
		// L03: TODO 3: Create and call a private function to load and fill all your map data
		LOG("Loading map data...");
		ret = LoadMapData(mapFile.child("map"));
	}

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	for (pugi::xml_node tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
	{
		if (ret == true)
		{
			TileSetInfo* info = new TileSetInfo; //TODO: LIMPIAR ESTE USO DE MEMORIA DINAMICA

			LOG("Loading tilesets data...");

			ret = LoadTileset(tileset, info);
			data.tilesets.add(info);
		}
		// L04: TODO 4: Iterate all layers and load each of them
		pugi::xml_node layer;
		for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* set2 = new MapLayer();

			if (ret == true) ret = LoadLayer(layer, set2);

			data.layers.add(set2);

		}

		if (ret == true)
		{
			// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
			LOG("Succesfully parsed XML file: %s", filename);
			LOG("width: %d height: %d", data.width, data.height);
			LOG("tile_width: %d tile_height %d", data.tileWidth, data.tileHeight);

			for (int i = 0; i < data.tilesets.count(); i++)
			{
				LOG("Tileset %d ----", i + 1);
				LOG("name: %s firstgid: %d", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstGid);
				LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->imageWidth, data.tilesets.At(i)->data->imageHeight);
				LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);
			}
		}

		mapLoaded = ret;

		return ret;
	}
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



	

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	// L04: TODO 3: Load a single layer
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
