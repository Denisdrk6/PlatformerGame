#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Log.h"
#include "Window.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack()
{
	
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	screenRect = { 0, 0, app->win->screenSurface->w * (int)app->win->GetScale(), app->win->screenSurface->h * (int)app->win->GetScale() };

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	Uint32 now = SDL_GetTicks() - start_time;

	float fadeRatio = MIN(1.0f, (float)now / (float)total_time);

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++start_time;
		if (start_time >= total_time)
		{
			moduleToDisable->Disable();  
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--start_time;
		if (start_time <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}
	if (currentStep == Fade_Step::FROM_BLACK)
	{
		fadeRatio = 1.0f - fadeRatio;
		if (now >= total_time)
			currentStep = Fade_Step::NONE;
	}

	

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);
	return true;
}



bool FadeToBlack::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		start_time = SDL_GetTicks() + 0.1f;
		total_time = (Uint32)(frames + 0.1f);

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}