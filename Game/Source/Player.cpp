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
	position.y = 95 * app->map->data.tileHeight;

	destroyed = false;

	collider = app->col->AddCollider({ (int)position.x, (int)position.y, 50, 64 }, COLLIDER_TYPE::COLLIDER_PLAYER, this);

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		// We could delete the equal frame condition to make the animation more accuret, but it would be less smooth
		if ((currentAnimation != &lJumpAnim && currentAnimation != &rJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1]) && groundCol == true))
		{
			currentAnimation = &lWalkAnim;
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

		// Camera can only move to the left if player is between tile width * 3 and the screen surface minus tile width * 11
		if ((position.x + app->render->camera.x) > app->map->data.tileWidth * 3 && (position.x + app->render->camera.x) < app->win->screenSurface->w - app->map->data.tileWidth * 11 && wallCol == false)
		{
			app->render->camera.x += speedX;
		}

		position.x -= speedX;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		// We change the animation to the right walking one if we are not jumping or falling or if we are on the las jumping frame and just touched a platform
		if ((currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1]) && groundCol == true))
		{
			currentAnimation = &rWalkAnim;
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

		// Camera can only move to the right if player is between tile width * 4 and the screen surface minus tile width * 11
		if ((position.x + app->render->camera.x) > app->map->data.tileWidth * 4 && (position.x + app->render->camera.x) < app->win->screenSurface->w - app->map->data.tileWidth * 4 && wallCol == false)
		{
			app->render->camera.x -= speedX;
		}

		position.x += speedX;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT && collider->type != COLLIDER_GODMODE)
	{
		// If sapce is pressed we trun of ground collision and turn on spacePressed var
		groundCol = false;
		spacePressed = true;
	}

	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && collider->type != COLLIDER_GODMODE)
	{
		// Double jump mechanic: if player has already jumped once and not double jumped or if player has fallen from platform and has not jumped on air, double jump turns true
		if ((spacePressed == true && doubleJump == false) || (spacePressed == false && doubleJump == false && speedY < 0))
		{
			doubleJump = true;
			speedY = 1.25f;
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT && collider->type == COLLIDER_GODMODE) position.y -= speedX;

	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && collider->type == COLLIDER_GODMODE)
	{
		position.y += speedX;
	}

	 //If no right/left/up movement detected, set the current animation back to idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_IDLE
		&& (currentAnimation != &rIdleAnim && currentAnimation != &lIdleAnim))
	{
		if (currentAnimation == &rWalkAnim || (currentAnimation == &rJumpAnim && groundCol == true) || (currentAnimation == &rFallAnim && groundCol == true))
		{
 			currentAnimation = &rIdleAnim;
		}
		else if (currentAnimation == &lWalkAnim || (currentAnimation == &lJumpAnim && groundCol == true) || (currentAnimation == &lFallAnim && groundCol == true))
		{
			currentAnimation = &lIdleAnim;
		}
	}

	// Camera can only go down if player is below the screen height minus 9 tiles
	if ((position.y + app->render->camera.y) >= app->win->screenSurface->h - (app->map->data.tileHeight * 9))
	{
		app->render->camera.y -= 1.45f;
	}

	if ((position.y + app->render->camera.y) < app->win->screenSurface->h / 4)
	{
		app->render->camera.y += 1.45f;
	}

	// Gravity
	if (groundCol == false && collider->type != COLLIDER_GODMODE)
	{
		// Cheks if player is falling from a platform instead of jumping
		if (spacePressed == false && doubleJump == false && speedY > 0) speedY = 0;

		// Reduces player speed each frame to make it smoother
		if (speedY >= -1.4f) speedY -= gravity;

		position.y -= speedY;

		// Activates jump animation
		if (speedY > 0 && currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim)
		{

			if (currentAnimation == &rWalkAnim || currentAnimation == &rIdleAnim || currentAnimation == &rFallAnim)
			{
				currentAnimation = &rJumpAnim;
				currentAnimation->Reset();
			}

			else if (currentAnimation == &lWalkAnim || currentAnimation == &lIdleAnim || currentAnimation == &lFallAnim)
			{
				currentAnimation = &lJumpAnim;
				currentAnimation->Reset();
			}
		}

		// Activates fall animation
		else if (speedY < 0)
		{
			if (currentAnimation == &rWalkAnim || currentAnimation == &rIdleAnim || currentAnimation == &rJumpAnim)
			{
				currentAnimation = &rFallAnim;
				currentAnimation->Reset();
			}

			else if (currentAnimation == &lWalkAnim || currentAnimation == &lIdleAnim || currentAnimation == &lJumpAnim)
			{
				currentAnimation = &lFallAnim;
				currentAnimation->Reset();
			}
		}
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
	if (collider->type != COLLIDER_GODMODE)
	{
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_DEAD)
		{
			currentAnimation = &rIdleAnim;
			position.x = 2 * app->map->data.tileWidth;
			position.y = 95 * app->map->data.tileHeight;
			app->render->camera.x = 0;
			app->render->camera.y = -77.5 * app->map->data.tileHeight;
			groundCol = true;
			firstFrame = true;
			spacePressed = false;
			doubleJump = false;
			speedY = 1.4f;
		}
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WALL)
		{
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - 10 && groundCol == false && speedY > 0)
			{
				speedY = -0.5f;
				position.y = c2->rect.y + c2->rect.h;
				downCol = true;
			}

			else if (((c1->rect.x + c1->rect.w) >= c2->rect.x) && (currentAnimation == &rWalkAnim || currentAnimation == &rJumpAnim || currentAnimation == &rFallAnim || currentAnimation == &rIdleAnim) && downCol == false)
				position.x = c2->rect.x - c1->rect.w;

			else if (c1->rect.x <= (c2->rect.x + c2->rect.w) && (currentAnimation == &lWalkAnim || currentAnimation == &lJumpAnim || currentAnimation == &lFallAnim || currentAnimation == &lIdleAnim) && downCol == false)
				position.x = c2->rect.x + c2->rect.w;

			wallCol = true;
		}

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
		{
			if (c2->rect.y - (c1->rect.h + c1->rect.y) >= -2) // -2 works as a way to try and have less tunneling
			{
				// Reset all jumping and falling variables
				if (currentAnimation == &rFallAnim && firstFrame == false) currentAnimation = &rJumpAnim;
				else if (currentAnimation == &lFallAnim && firstFrame == false) currentAnimation = &lJumpAnim;

				// Avoids jumping animation when respawning
				else if (firstFrame == false && groundCol == true && currentAnimation != &lWalkAnim && currentAnimation != &lIdleAnim && currentAnimation != &lFallAnim && currentAnimation != &lJumpAnim)
					currentAnimation = &rIdleAnim;

				groundCol = true;
				spacePressed = false;
				doubleJump = false;
				speedY = 1.45f;
			}
		}
	}
}