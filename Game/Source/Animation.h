#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 500

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

public:
	float currentFrame;
	int lastFrame = 0;
	int finished = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[lastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		currentFrame += speed;
		if (currentFrame >= lastFrame) {
			currentFrame = 0;
			finished = 1;
		}
		else finished = 0;

		return frames[(int)currentFrame];
	}
	int GetFinished() { return finished; }
	void Finish() { finished = 1; }
	void Reset() { currentFrame = 0; finished = 0; }
};

#endif
