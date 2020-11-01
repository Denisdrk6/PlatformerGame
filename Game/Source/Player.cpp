#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"
#include "Collisions.h"
#include "Log.h"
#include "Map.h"
#include "Window.h"

#include <stdio.h>

Player::Player() : Module()
{
	name.Create("player");

	rIdleAnim.PushBack({ 0, 136, 32, 32 });
	rIdleAnim.PushBack({ 32, 136, 32, 32 });
	rIdleAnim.PushBack({ 64, 136, 32, 32 });
	rIdleAnim.PushBack({ 96, 136, 32, 32 });
	rIdleAnim.speed = 0.02f;

	lIdleAnim.PushBack({ 96, 168, 32, 32 });
	lIdleAnim.PushBack({ 64, 168, 32, 32 });
	lIdleAnim.PushBack({ 32, 168, 32, 32 });
	lIdleAnim.PushBack({ 0, 168, 32, 32 });
	lIdleAnim.speed = 0.02f;

	rWalkAnim.PushBack({ 0, 70, 32, 32 });
	rWalkAnim.PushBack({ 32, 70, 32, 32 });
	rWalkAnim.PushBack({ 64, 70, 32, 32 });
	rWalkAnim.PushBack({ 96, 70, 32, 32 });
	rWalkAnim.PushBack({ 128, 70, 32, 32 });
	rWalkAnim.PushBack({ 160, 70, 32, 32 });
	rWalkAnim.speed = 0.1f;

	lWalkAnim.PushBack({ 160, 104, 32, 32 });
	lWalkAnim.PushBack({ 128, 104, 32, 32 });
	lWalkAnim.PushBack({ 96, 104, 32, 32 });
	lWalkAnim.PushBack({ 64, 104, 32, 32 });
	lWalkAnim.PushBack({ 32, 104, 32, 32 });
	lWalkAnim.PushBack({ 0, 104, 32, 32 });
	lWalkAnim.speed = 0.1f;

	rJumpAnim.PushBack({ 0, 0, 32, 32 });
	rJumpAnim.PushBack({ 32, 0, 32, 32 });
	rJumpAnim.PushBack({ 64, 0, 32, 32 });
	rJumpAnim.PushBack({ 96, 0, 32, 32 });
	rJumpAnim.PushBack({ 128, 0, 32, 32 });
	rJumpAnim.PushBack({ 160, 0, 32, 32 });
	rJumpAnim.PushBack({ 192, 0, 32, 32 });
	rJumpAnim.PushBack({ 224, 0, 32, 32 });
	rJumpAnim.speed = 0.025f;

	lJumpAnim.PushBack({ 224, 36, 32, 32 });
	lJumpAnim.PushBack({ 192, 36, 32, 32 });
	lJumpAnim.PushBack({ 160, 36, 32, 32 });
	lJumpAnim.PushBack({ 128, 36, 32, 32 });
	lJumpAnim.PushBack({ 96, 36, 32, 32 });
	lJumpAnim.PushBack({ 64, 36, 32, 32 });
	lJumpAnim.PushBack({ 32, 36, 32, 32 });
	lJumpAnim.PushBack({ 0, 36, 32, 32 });
	lJumpAnim.speed = 0.025f;

	rFallAnim.PushBack({ 128, 0, 32, 32 });
	rFallAnim.PushBack({ 160, 0, 32, 32 });
	rFallAnim.speed = 0.015f;

	lFallAnim.PushBack({ 96, 36, 32, 32 });
	lFallAnim.PushBack({ 64, 36, 32, 32 });
	lFallAnim.speed = 0.015f;
}

Player::~Player()
{

}

bool Player::Awake()
{
	return true;
}

bool Player::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = app->tex->Load("Assets/player/player.png");
	currentAnimation = &rIdleAnim;

	laserFx = app->audio->LoadFx("Assets/Fx/laser.wav");

	explosionFx = app->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 2 * app->map->data.tileWidth;
	position.y = 89 * app->map->data.tileHeight;

	destroyed = false;

	collider = app->col->AddCollider({ position.x, position.y, 50, 64 }, COLLIDER_TYPE::COLLIDER_PLAYER, this);

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;


	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		
		if ((currentAnimation != &lJumpAnim && currentAnimation != &rJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1])))
		{
			currentAnimation = &lWalkAnim;
			jump = false;
		}

		else if (currentAnimation == &rJumpAnim)
		{
			float frame = currentAnimation->current_frame;
			currentAnimation = &lJumpAnim;
			currentAnimation->current_frame = frame;
		}

		else if (currentAnimation == &rFallAnim)
		{
			float frame = currentAnimation->current_frame;
			currentAnimation = &lFallAnim;
			currentAnimation->current_frame = frame;
		}

		if (position.x > app->win->screenSurface->w / 12 && position.x < 14* app->win->screenSurface->w / 11 && wallCol == false)
		{
			app->render->camera.x += speed;
		}

		position.x -= speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		
		if ((currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1])))
		{
			currentAnimation = &rWalkAnim;
			jump = false;
		}

		else if (currentAnimation == &lJumpAnim)
		{
			float frame = currentAnimation->current_frame;
			currentAnimation = &rJumpAnim;
			currentAnimation->current_frame = frame;
		}

		else if (currentAnimation == &lFallAnim)
		{
			float frame = currentAnimation->current_frame;
			currentAnimation = &rFallAnim;
			currentAnimation->current_frame = frame;
		}


		if (position.x > app->win->screenSurface->w / 12 && wallCol == false)
		{
			app->render->camera.x -= speed;
		}

		position.x += speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT && jump == false && doubleJump == false)
	{
		if (currentAnimation == &rFallAnim || currentAnimation == &lFallAnim) doubleJump = true;
		jump = true;
		fall = false;
	}

	if (jump == true && fall == false)
	{
		if (currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim)
		{

			if (currentAnimation == &rWalkAnim || currentAnimation == &rIdleAnim || currentAnimation == &rFallAnim)
			{
				currentAnimation = &rJumpAnim; 
				currentAnimation->Reset();
			}

			else
			{
				currentAnimation = &lJumpAnim;
				currentAnimation->Reset();
			}
		}

		position.y -= speed * 2;

		if ((position.y + app->render->camera.y) < app->win->screenSurface->h / 4)
		{
			app->render->camera.y += speed * 2;
		}
	}

	 //If no right/left/up movement detected, set the current animation back to idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_IDLE
		&& (currentAnimation != &rIdleAnim && currentAnimation != &lIdleAnim))
	{
		if (currentAnimation == &rWalkAnim || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1])) || (currentAnimation == &rFallAnim && fall == false))
		{
			currentAnimation = &rIdleAnim;
			jump = false;
		}
		else if (currentAnimation == &lWalkAnim || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1])) || (currentAnimation == &lFallAnim && fall == false))
		{
			currentAnimation = &lIdleAnim;
			jump = false;
		}
	}

	//Gravity
	if ((currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[4]) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[4]))) || fall == true)
	{
		if (fall == true)
		{
			position.y += speed;
			if (currentAnimation == &rJumpAnim || currentAnimation == &rWalkAnim) currentAnimation = &rFallAnim;
			else if (currentAnimation == &lJumpAnim || currentAnimation == &lWalkAnim) currentAnimation = &lFallAnim;
			jump = false;
		}

		fall = true;
	}

	if ((position.y + app->render->camera.y) > 3 * app->win->screenSurface->h / 5)
	{
		app->render->camera.y -= speed * 2;
	}

	collider->SetPos(position.x, position.y);

	return ret;
}

bool Player::EqualFrames(SDL_Rect frame1, SDL_Rect frame2)
{
	return (frame1.x == frame2.x && frame1.y == frame2.y && frame1.w == frame2.w && frame1.h == frame2.h);
}

bool Player::PostUpdate()
{
	bool ret = false;

	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		ret = true;
	}

	return ret;
}

bool Player::CleanUp()
{

	app->tex->UnLoad(texture);

	app->col->DeleteCollider(collider);

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WALL)
	{
		if (((c1->rect.x + c1->rect.w) > c2->rect.x) && (currentAnimation == &rWalkAnim || currentAnimation == &rJumpAnim || currentAnimation == &rFallAnim))
			position.x = c2->rect.x - c1->rect.w;

		else if(c1->rect.x < (c2->rect.x + c2->rect.w))
			position.x = c2->rect.x + c2->rect.w;

		wallCol = true;
	}

	if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
	{
		if (c2->rect.y - (c1->rect.h + c1->rect.y) >= -1)
		{
			fall = false;
			if (doubleJump == true && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim && currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim) doubleJump = false;
			if (currentAnimation == &rFallAnim) currentAnimation = &rJumpAnim;
			else if (currentAnimation == &lFallAnim) currentAnimation = &lJumpAnim;
		}
	}
}