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

    int firstGid;
    SString name;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
	int	numTilesWidth;
	int	numTilesHeight;

    SString source;
    int imageWidth;
    int imageHeight;

    SDL_Texture* texture;
	
	// L04: TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
    MAPTYPE_UNKNOWN = 0,
    MAPTYPE_ORTHOGONAL,
    MAPTYPE_ISOMETRIC,
    MAPTYPE_STAGGERED
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
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
	// L04: TODO 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
    List<Collider*>colliders;
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

    void OnCollision(Collider* c1, Collider* c2);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
    // L03: TODO 1: Add your struct for map info as public for now
    MapInfo data;

private:

    pugi::xml_document mapFile;
    SString folder;
    bool mapLoaded;

    bool LoadMapData(pugi::xml_node);
    bool LoadTileset(pugi::xml_node, TileSetInfo*);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

};

#endif // __MAP_H__