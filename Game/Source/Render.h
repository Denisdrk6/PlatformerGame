#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, int mult = 1, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	iPoint ScreenToWorld(int x, int y) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
};

#endif // __RENDER_H__