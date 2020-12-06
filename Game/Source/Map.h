#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"



#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!


struct TileSetInfo
{

    int         firstGid;
    SString     name;
    int         tileWidth;
    int         tileHeight;
    int         spacing;
    int         margin;
	int	        numTilesWidth;
	int	        numTilesHeight;

    SString     source;
    int         imageWidth;
    int         imageHeight;

    SDL_Texture*texture;
	
	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
    MAPTYPE_UNKNOWN = 0,
    MAPTYPE_ORTHOGONAL,
    MAPTYPE_ISOMETRIC,
    MAPTYPE_STAGGERED
};

struct Properties
{
    struct Property
    {
        SString name;
        SString type;
        int value;
        float f_value;
    };

    ~Properties()
    {
        ListItem<Property*>* item;
        item = list.start;

        while (item != NULL)
        {
            RELEASE(item->data);
            item = item->next;
        }

        list.Clear();
    }

    // L06: DONE 7: Method to ask for the value of a custom property
    int GetProperty(const char* name, int default_value = 0) const;
    float GetPropertyF(const char* name, int default_value = 0) const;
    List<Property*> list;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	    name;
	int         width;
	int         height;
	uint*       data;
    //int         tilesetNum;

    Properties properties;

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
    SString             orientation;
    SString             renderOrder;
    int                 width;
    int                 height;
    int                 tileWidth;
    int                 tileHeight;
    int                 nextObjectId;
    MapTypes            type;
    List<TileSetInfo*>  tilesets;
	List<MapLayer*>     layers;
    List<Collider*>     colliders;
    List<ObjectLayer*>  obj_layers;
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

    bool    collisionDraw = false;

   
private:

    pugi::xml_document      mapFile;
    SString                 folder;
    bool                    mapLoaded;

    bool LoadMapData(pugi::xml_node);
    bool LoadTileset(pugi::xml_node, TileSetInfo*);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
    bool LoadObjLayer(pugi::xml_node& node, ObjectLayer* layer);
    // L06: DONE 6: Load a group of properties 
    bool LoadProperties(pugi::xml_node& node, Properties& properties);

    TileSetInfo* GetTilesetFromTileId(int id) const;

};

#endif // __MAP_H__