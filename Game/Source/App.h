#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class Animation;
class Player;
class Collisions;
class FadeToBlack;
class SceneIntro;
class SceneWin;
class Fonts;
class PathFinding;
class Entities;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void ChangeFrameCap(int cap);

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	//load/save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Map* map;
	Animation* animation;
	Player* player;
	Collisions* col;
	FadeToBlack* fade;
	SceneIntro* intro;
	SceneWin* winScene;
	Fonts* fonts;
	PathFinding* pathfinding;
	Entities* entities;

	int maxFramerate = 0;
	bool fpsCapped;
	int fpsN;
	int fpsA;
	int lastMs;
	
	uint16_t framerate = 1000 / 45;
	
	float dt;

private:

	float DeltaTime;

	int argc;
	char** args;
	SString title;
	SString organization;


	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	//pugi::xml_document configFile;
	//pugi::xml_node config;
	//pugi::xml_node configApp;

	
	// the filename for save / load
	uint frames;
	mutable bool saveGameRequested;
	bool loadGameRequested;
	/*SString loadedGame;
	mutable SString savedGame;*/

	PerfTimer ptimer;
	uint frameCount = 0;
	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint lastSecFrameCount = 0;
	uint prevLastSecFrameCount = 0;

	
	float maxFrameMs;
};

extern App* app;

#endif	// __APP_H__