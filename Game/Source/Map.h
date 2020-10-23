#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct TileSetInfo
{
   /* pugi::xml_node tileSet;
    int firstGid = tileSet.attribute("firstgid").as_int();
    SString name = tileSet.attribute("Desert").as_string();
    int tileWidth = tileSet.attribute("tilewidth").as_int();
    int tileHeight = tileSet.attribute("tilehright").as_int();
    int spacing = tileSet.attribute("spacing").as_int();
    int margin = tileSet.attribute("margin").as_int();

    pugi::xml_node image = tileSet.child("image");
    SString source = image.attribute("source").as_string();
    int imageWidth = image.attribute("width").as_int();
    int imageHeight = image.attribute("height").as_int();*/

    int firstGid;
    SString name;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;

    SString source;
    int imageWidth;
    int imageHeight;

    SDL_Texture* tilesetTexture;
};

enum MapTypes
{
    MAPTYPE_UNKNOWN = 0,
    MAPTYPE_ORTHOGONAL,
    MAPTYPE_ISOMETRIC,
    MAPTYPE_STAGGERED
};

// L03: TODO 1: Create a struct needed to hold the information to Map node

struct MapInfo
{
    SString orientation;
    SString renderOrder;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    int nextObjectId;
    MapTypes type;
    List<TileSetInfo*> tilesets;
};


class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* path);

    // L03: TODO 1: Add your struct for map info as public for now
    MapInfo mapInfo;

private:

    pugi::xml_document mapFile;
    SString folder;
    bool mapLoaded;

    bool LoadMapData(pugi::xml_node);
    bool LoadTileset(pugi::xml_node, TileSetInfo*);
};

#endif // __MAP_H__