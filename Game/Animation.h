#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 50000

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

public:
	float current_frame;
	int last_frame = 0;
	int finished = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame) {
			current_frame = 0;
			finished = 1;
		}
		else finished = 0;

		return frames[(int)current_frame];
	}
	int GetFinished() { return finished; }
	void Finish() { finished = 1; }
	void Reset() { current_frame = 0;	finished = 0; }
};

#endif