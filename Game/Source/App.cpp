#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
//#include "SceneIntro.h"
//#include "SceneGameplay.h"
//#include "SceneWin.h"
#include "SceneManager.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "Collisions.h"
#include "Fonts.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Entities.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);
	frames = 0;

	
	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	//gameplay = new SceneGameplay();
	map = new Map();
	player = new Player();
	col = new Collisions();
	fade = new FadeToBlack();
	sceneManager = new SceneManager(input, render, tex);
	//intro = new SceneIntro();
	//winScene = new SceneWin();
	fonts = new Fonts();
	pathfinding = new PathFinding();
	entities = new Entities();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	/*AddModule(intro);
	AddModule(gameplay);
	AddModule(winScene);*/
	AddModule(sceneManager);
	AddModule(fonts);
	AddModule(map);
	AddModule(player);
	AddModule(entities);
	AddModule(col);
	AddModule(pathfinding);
	AddModule(fade);

	// Render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();

	//configFile.reset();
}

void App::AddModule(Module* module)
{
	bool initialState = true;
	if (module == map || module == audio || module == col || module == player) initialState = false;
	module->Init(initialState);
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		// 1: Read from config file your framerate cap
		maxFramerate = configApp.attribute("framerate_cap").as_int();

		if (maxFramerate != 0) {
			maxFrameMs = 1000.0f * (1 / (float)maxFramerate);
			fpsCapped = true;
		}
	}

	if (ret == true)
	{
		

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	startupTime.Start();

	PERF_PEEK(ptimer);

	return ret;
}
void App::ChangeFrameCap(int cap) {
	maxFramerate = cap;
	fpsCapped = true;

	if (maxFramerate != 0) {
		maxFrameMs = 1000.0f * (1 / (float)maxFramerate);

	}
	else if (maxFramerate == 0) {
		maxFrameMs = NULL;
		fpsCapped = false;
	}
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		pauseMenu = !pauseMenu;
	}

	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();

	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// 4: Calculate the dt: differential time since last frame
	DeltaTime = frameTime.ReadSec();
	frameTime.Start();
	ptimer.Start();
}


// ---------------------------------------------
void App::FinishUpdate()
{
	if (saveGameRequested == true)SaveGame();
	if (loadGameRequested == true) LoadGame();


	// Framerate calculations --

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float avgFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint lastFrameMs = frameTime.Read();
	uint framesOnLastUpdate = prevLastSecFrameCount;

	SString frameCapTitle;
	if (fpsCapped == true) {
		frameCapTitle = "ON";
	}
	else if (fpsCapped == false) {
		frameCapTitle = "OFF";
	}


	/*static char title[256];
	sprintf_s(title, 256, "FPS: %i / Av.FPS: %.2f / Last Frame Ms: %02u (Frame Cap: %s) ",
		frames_on_last_update, avg_fps, last_frame_ms, frame_cap_title.GetString());
	app->win->SetTitle(title);*/

	// 2: Use SDL_Delay to make sure you get your capped framerate
	if (lastFrameMs < maxFrameMs && framerate > 0) {
		PerfTimer delayMs;
		delayMs.Start();
		SDL_Delay(maxFrameMs - lastFrameMs);
		//LOG("We waited for %i miliseconds and got back in %f", (int)max_frame_ms, Delay_ms.ReadMs());
	}
	// 3: Measure accurately the amount of time it SDL_Delay actually waits compared to what was expected
	fpsN = framesOnLastUpdate;
	fpsA = avgFps;
	lastMs = lastFrameMs;

}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if(pauseMenu == false || (pauseMenu == true && (item->data->name == "input" || item->data->name == "renderer" || item->data->name == "scenemanager")))
			ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

      	else if (pauseMenu == false || (pauseMenu == true && (item->data->name == "input" || item->data->name == "renderer" || item->data->name == "scenemanager")))
			ret = item->data->Update(DeltaTime);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if (pauseMenu == false || (pauseMenu == true && (item->data->name == "input" || item->data->name == "renderer" || item->data->name == "scenemanager")))
			ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// then call all the modules to load themselves

void App::LoadGameRequest() 
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}


void App::SaveGameRequest()const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;

}

bool App::LoadGame()
{
	bool ret = false;
	pugi::xml_document data;
	pugi::xml_node root;
	//loadedGame = "load_game.xml";
	pugi::xml_parse_result result = data.load_file(SAVE_STATE_FILENAME);

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", SAVE_STATE_FILENAME);

		root = data.child("game_state");

		ListItem<Module*>* item = modules.start;
		ret = true;
		while (item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			//if (item->data->name.GetString() == "player" ) int externMap = app->player->map;
			LOG("Name: %s", item->data->name.GetString());
			item = item->next;
		}

		data.reset();
		if (ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", SAVE_STATE_FILENAME, result.description());

	loadGameRequested = false;
	return ret;
	}
bool App::SaveGame() const
{
	bool ret = true;
	//savedGame = "savedame.xml";

	LOG("Saving Game State to %s...", SAVE_STATE_FILENAME);

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	if(data.empty() == false)
		data.reset();

	root = data.append_child("game_state");

	ListItem<Module*>* item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret == true)
	{
		data.save_file(SAVE_STATE_FILENAME);
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	saveGameRequested = false;
	return ret;
}