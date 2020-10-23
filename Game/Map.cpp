
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

void Map::Draw()
{
    if (mapLoaded == false)
        return;

    // L03: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)
    for (int i = 0; i < mapInfo.tilesets.count(); i++)
    {
        SDL_Rect tilesetSize = { 0, 0, mapInfo.tilesets.At(i)->data->imageWidth, mapInfo.tilesets.At(i)->data->imageHeight };
        app->render->DrawTexture(mapInfo.tilesets.At(i)->data->tilesetTexture, 0, 0, &tilesetSize);
    }

}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map


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

    // L03: TODO 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
    for (pugi::xml_node tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
    {
        if (ret == true)
        {
            TileSetInfo* info = new TileSetInfo; //TODO: LIMPIAR ESTE USO DE MEMORIA DINAMICA

            LOG("Loading tilesets data...");

            ret = LoadTileset(tileset, info);
            mapInfo.tilesets.add(info);
        }
    }

    if (ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
        LOG("Succesfully parsed XML file: %s", filename);
        LOG("width: %d height: %d", mapInfo.width, mapInfo.height);
        LOG("tile_width: %d tile_height %d", mapInfo.tileWidth, mapInfo.tileHeight);

        for (int i = 0; i < mapInfo.tilesets.count(); i++)
        {
            LOG("Tileset %d ----", i + 1);
            LOG("name: %s firstgid: %d", mapInfo.tilesets.At(i)->data->name.GetString(), mapInfo.tilesets.At(i)->data->firstGid);
            LOG("tile width: %d tile height: %d", mapInfo.tilesets.At(i)->data->imageWidth, mapInfo.tilesets.At(i)->data->imageHeight);
            LOG("spacing: %d margin: %d", mapInfo.tilesets.At(i)->data->spacing, mapInfo.tilesets.At(i)->data->margin);
        }
    }

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMapData(pugi::xml_node mapData)
{
    bool ret = true;

    if (mapData == NULL) ret = false;

    mapInfo.orientation = mapData.attribute("orientation").as_string();
    mapInfo.renderOrder = mapData.attribute("renderorder").as_string();
    mapInfo.width = mapData.attribute("width").as_int();
    mapInfo.height = mapData.attribute("height").as_int();
    mapInfo.tileWidth = mapData.attribute("tilewidth").as_int();
    mapInfo.tileHeight = mapData.attribute("tileheight").as_int();
    mapInfo.nextObjectId = mapData.attribute("nextobjectid").as_int();

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

    SString tmp("%s%s", folder.GetString(), info->source.GetString());
    info->tilesetTexture = app->tex->Load(tmp.GetString());

    return ret;
}



