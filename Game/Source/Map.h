#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

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
    //Properties properties;
	uint* data;
    //int         tilesetNum;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct ObjectData {
    SString name;
    int x;
    int y;
    int w;
    int h;
};

struct ObjectLayer {
    SString name;
    List<ObjectData*> list;
};

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
	List<MapLayer*> layers;
    List<Collider*> colliders;
    List<ObjectLayer*> objLayers;
};


class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    //bool Start();

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* path);

    void OnCollision(Collider* c1, Collider* c2);

	iPoint MapToWorld(int x, int y) const;
    iPoint WorldToMap(int x, int y) const;

    bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

    MapInfo data;

    bool collisionDraw = false;

private:

    pugi::xml_document mapFile;
    SString folder;
    bool mapLoaded;

    bool LoadMapData(pugi::xml_node);
    bool LoadTileset(pugi::xml_node, TileSetInfo*);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
    bool LoadObjLayer(pugi::xml_node& node, ObjectLayer* layer);

    TileSetInfo* GetTilesetFromTileId(int id) const;

};

#endif // __MAP_H__