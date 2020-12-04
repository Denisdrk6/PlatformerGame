#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"
#include "Collisions.h"
#include "Fonts.h"
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


	rShootAnim.PushBack({ 264,6,21,26});
	rShootAnim.PushBack({ 296,7,22,25 });
	rShootAnim.PushBack({ 330,5,20,27 });
	rShootAnim.PushBack({ 360,5,21,27 });
	rShootAnim.speed = 0.02f;


	lShootAnim.PushBack({ 360,40,21,26 });
	lShootAnim.PushBack({ 327,41,22,25 });
	lShootAnim.PushBack({ 295,39,20,27 });
	lShootAnim.PushBack({ 264,39,21,27 });
	lShootAnim.speed = 0.02f;

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
	heart = app->tex->Load("Assets/textures/heart.png");
	textureHurt.texture = app->tex->Load("Assets/player/playerHurt.png");
	textureHurt.loaded = false;
	currentAnimation = &rIdleAnim;

	position.x = 3 * 32; //app->map->data.tileWidth;
	position.y = 95 * 32; //app->map->data.tileHeight;
	app->render->camera.x = 0;
	app->render->camera.y = -77.5 * app->map->data.tileHeight;

	destroyed = false;

	collider = app->col->AddCollider({ (int)position.x, (int)position.y, 50, 64 }, COLLIDER_TYPE::COLLIDER_PLAYER, this);
	char lookupTable[] = { "01234567/ " };
	scoreFont = app->fonts->Load("Assets/Fonts/score_font.png", lookupTable, 1);
	return ret;
}
void Player::Death() {

	bool loaded = false;
	for (int i = 1; i >= 0; i--)
	{
		if (app->scene->checkPoints[i].activated == true)
		{
			app->LoadGameRequest();
			loaded = true;
			groundCol = true;
			firstFrame = true;
			spacePressed = false;
			doubleJump = false;
			speedY = 1.4f;
			currentAnimation = &rIdleAnim;
		}
	}

	if (loaded == false)
	{
		currentAnimation = &rIdleAnim;
		position.x = 2 * app->map->data.tileWidth;
		position.y = spawnX * app->map->data.tileHeight;
		app->render->camera.x = 0;
		app->render->camera.y = -77.5 * app->map->data.tileHeight;
		groundCol = true;
		firstFrame = true;
		spacePressed = false;
		doubleJump = false;
		speedY = 1.4f;
	}
	app->audio->PlayFx(1);
	lifes = 3;
}
bool Player::Update(float dt)
{
	bool ret = true;
	if (position.x > app->map->data.tileWidth * app->map->data.width || position.x < 0) {
		lifes = 0;
		Death();
	}

	if (position.y>app->map->data.height*app->map->data.tileHeight) {
		lifes = 0;
		Death();
	}

	delta = dt;

	// Change speeds with dt
	speedX = dt * speedMultiplier;
	if (speedX - (int)speedX >= 0.5f) speedX = (int)speedX + 1;
	else speedX = (int)speedX;

	if (speedY == 1.45f)
	{
		speedY = 1.45f * dt * speedMultiplier;
		maxNegativeSpeedY = (speedY * -1) + (0.05f * dt * speedMultiplier);
		gravity = (speedY / 145) * dt * speedMultiplier;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		// We could delete the equal frame condition to make the animation more accuret, but it would be less smooth
		if ((currentAnimation != &lJumpAnim && currentAnimation != &rJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1]) && groundCol == true)) {
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
		// We change the animation to the right walking one if we are not jumping or falling or if we are on the las jumping frame and just touched a platform		if ((currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1])))
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
			speedY = 1.25f * dt * speedMultiplier;
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT && collider->type == COLLIDER_GODMODE)
	{
		position.y -= speedX;
		if ((position.y + app->render->camera.y) < app->win->screenSurface->h / 4)app->render->camera.y += 1.45 * dt * speedMultiplier;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && collider->type == COLLIDER_GODMODE)
	{
		position.y += speedX;
		if ((position.y + app->render->camera.y) >= app->win->screenSurface->h - (app->map->data.tileHeight * 9)) app->render->camera.y -= 1.45 * dt * speedMultiplier;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN)
	{
		if (currentAnimation == &rIdleAnim || currentAnimation == &rJumpAnim || currentAnimation == &rWalkAnim || currentAnimation == &rFallAnim)

			currentAnimation == &rShootAnim;


		else if (currentAnimation == &lIdleAnim || currentAnimation == &lJumpAnim || currentAnimation == &lWalkAnim || currentAnimation == &lFallAnim)

			currentAnimation == &lShootAnim;
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
	if ((position.y + app->render->camera.y) >= app->win->screenSurface->h - (app->map->data.tileHeight * 9) && collider->type != COLLIDER_GODMODE)
	{
		// mov reduces shaking when moving camera upwards
		int mov = 0;
		if (speedY - (int)speedY >= 0.5f) mov = (int)speedX + 1;
		else mov = (int)speedY;

		if (speedY < 0)app->render->camera.y += mov;
	}

	if ((position.y + app->render->camera.y) < app->win->screenSurface->h / 4 && collider->type != COLLIDER_GODMODE)
	{
		if (speedY > 0)app->render->camera.y += speedY;
	}

	// Gravity
	if (groundCol == false && collider->type != COLLIDER_GODMODE)
	{
		// Cheks if player is falling from a platform instead of jumping
		if (spacePressed == false && doubleJump == false && speedY > 0) speedY = 0;

		// Reduces player speed each frame to make it smoother
		if (speedY >= maxNegativeSpeedY) speedY -= gravity;

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
	sprintf_s(scoreText, 10, "%i/%i",score, maxScore);

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
		app->fonts->BlitText(10, 10, scoreFont, scoreText);

		if(textureHurt.loaded == true)
		{
			textureHurt.alpha -= 1;
			textureHurt.rect = currentAnimation->GetCurrentFrame();

			if (textureHurt.alpha <= 0)
				textureHurt.loaded = false;

			else
			{
				//Set alpha value to texture and render it
				SDL_SetTextureAlphaMod(textureHurt.texture, textureHurt.alpha);
				app->render->DrawTexture(textureHurt.texture, position.x, position.y, &textureHurt.rect);
			}
		}

		// Lifes texture rendering
		for (int i = 0, j = 0; i < lifes; ++i, j += 30)
		{
			app->render->DrawTexture(heart, (app->render->camera.x * -1) + 1180 - (30 * lifes) + j, app->render->camera.y * -1 + 25);
		}

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

void Player::ChangeMap(int mapNum)
{
	map = mapNum;
	app->map->Disable();
	app->map->Enable();
	//app->map->data.tilesets.start->data->texture = app->tex->Load("");

	switch (map)
	{
	case 1:
		break;
	case 2:
		app->map->Load("Devmap2.tmx");
		spawnX = 96;
		position.x = 3 * 32;
		position.y = spawnX * 32;
		app->render->camera.x = 0;
		app->render->camera.y = -77.5 * app->map->data.tileHeight;
		currentAnimation = &rIdleAnim;
		groundCol = true;
		firstFrame = true;
		spacePressed = false;
		doubleJump = false;
		speedY = 1.4f;
		app->scene->checkPoints[0].position = {92, 46};
		app->scene->checkPoints[0].activated = false;
		app->col->DeleteCollider(app->scene->checkPoints[0].collider);
		app->scene->checkPoints[0].collider = app->col->AddCollider({ app->scene->checkPoints[0].position.x * app->map->data.tileWidth, app->scene->checkPoints[0].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);
		app->scene->checkPoints[1].position = { 0, 0 };
		app->scene->checkPoints[1].activated = false;
		app->col->DeleteCollider(app->scene->checkPoints[1].collider);

		app->scene->coins[0].position = {34, 93};
		app->scene->coins[0].activated = false;
		app->scene->coins[1].position = {91, 72};
		app->scene->coins[1].activated = false;
		app->scene->coins[2].position = {81, 56};
		app->scene->coins[2].activated = false;
		app->scene->coins[3].position = {82, 24};
		app->scene->coins[3].activated = false;
		app->scene->coins[4].position = {70, 13};
		app->scene->coins[4].activated = false;
		app->scene->coins[5].position = {51, 2};
		app->scene->coins[5].activated = false;
		app->scene->coins[6].position = {36, 9};
		app->scene->coins[6].activated = false;

		for (int i = 0; i < maxScore; i++)
			app->scene->coins[i].collider = app->col->AddCollider({ app->scene->coins[i].position.x * app->map->data.tileWidth, app->scene->coins[i].position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight}, COLLIDER_TYPE::COLLIDER_COIN, this);

		app->scene->hearts.position = {78, 42};
		app->scene->hearts.activated = false;
		app->scene->hearts.collider = app->col->AddCollider({ app->scene->hearts.position.x * app->map->data.tileWidth, app->scene->hearts.position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, this);
		
		score = 0;
		break;
	default:
		break;
	}
}



bool Player::CheckTunneling(SDL_Rect r1, SDL_Rect r2)
{
	return (r1.y > r2.y&& r1.y + r1.h < r2.y + r2.h);
}
void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (collider->type != COLLIDER_GODMODE)
	{
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_DEAD)
		{
			if (waiting == false && lifeTaken == false)
			{
				if (lifeWait == 0)
				{
					lifes--;
					lifeTaken = true;
					app->audio->PlayFx(1);
				}
				lifeWait = SDL_GetTicks();
				waiting = true;

				if (lifes != 0)
				{
					textureHurt.alpha = 255;
					textureHurt.loaded = true;
				}



				LOG("lifes: %d", lifes);
			}

			else if (lifeWait != 0)
			{
				int currentTime = SDL_GetTicks();

				if (currentTime - lifeWait >= 1500)
				{
					lifes--;
					waiting = false;
					LOG("lifes: %d", lifes);
					lifeWait = SDL_GetTicks();
					app->audio->PlayFx(1);

					if (lifes != 0)
					{
						textureHurt.alpha = 255;
						textureHurt.loaded = true;
					}
				}
			}

			if (lifes == 0)
				Death();


		}
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WALL)
		{
			fPoint relativePosition = position;
			// Checks if we are colliding a wall from below
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - 10 && groundCol == false && speedY > 0 && wallCol == false)
			{
				savedSpeed = speedY;
				speedY = -0.2f;
				position.y = c2->rect.y + c2->rect.h;
				downCol = true;
			}

			else if (((c1->rect.x + c1->rect.w) >= c2->rect.x) && (currentAnimation == &rWalkAnim || currentAnimation == &rJumpAnim || currentAnimation == &rFallAnim || currentAnimation == &rIdleAnim))
			{
				position.x = c2->rect.x - c1->rect.w;
				if (downCol == true)
				{
					downCol = false;
					if (speedY < 0) speedY = savedSpeed;
				}
			}

			else if (c1->rect.x <= (c2->rect.x + c2->rect.w) && (currentAnimation == &lWalkAnim || currentAnimation == &lJumpAnim || currentAnimation == &lFallAnim || currentAnimation == &lIdleAnim))
			{
				position.x = c2->rect.x + c2->rect.w;
				if (downCol == true)
				{
					downCol = false;
					speedY *= -1;
				}
			}

			wallCol = true;
		}

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
		{
			int offset = -2 - (120 / (1 / delta));
			if (offset < -7) offset = -7;
			SDL_Rect nextCollision = { c1->rect.x, c1->rect.y + speedY, c1->rect.w, c1->rect.h };

			if (CheckTunneling(nextCollision, c2->rect) == true || c2->rect.y - (c1->rect.h + c1->rect.y) >= offset) // offset works as a way to try and have less tunneling, it is bigger with lower fps and can be as low as 7		
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

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WIN)
			ChangeMap(map + 1);

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_CHECKPOINT)
		{
			for (int i = 0; i < 2; i++)
			{
				// last && condition prevents saving if there's a further checkpoint already activated
				if (c2->rect.x == app->scene->checkPoints[i].position.x * app->map->data.tileWidth && c2->rect.y == app->scene->checkPoints[i].position.y * app->map->data.tileHeight + 16 && app->scene->checkPoints[i].activated == false && ((i == 0 && app->scene->checkPoints[i + 1].activated == false) || i == 1))
				{
					app->scene->checkPoints[i].activated = true;
					app->SaveGameRequest();

					app->scene->saveTex.rect = { 0, 0, 224, 83 };
					if (app->scene->saveTex.loaded == false) app->scene->saveTex.texture = app->tex->Load("Assets/textures/saveLoad.png");
					app->scene->saveTex.alpha = 255;
					app->scene->saveTex.loaded = true;
				}
			}
		}
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_COIN)
		{

			for (int i = 0; i < 7; i++)
			{
				// last && condition prevents saving if there's a further checkpoint already activated
				if (c2->rect.x == app->scene->coins[i].position.x * app->map->data.tileWidth && c2->rect.y == app->scene->coins[i].position.y * app->map->data.tileHeight && app->scene->coins[i].activated == false)
				{
					score++;
					app->scene->coins[i].activated = true;
					app->col->DeleteCollider(app->scene->coins[i].collider);
				}
			}
		}
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_HEART)
		{

			// last && condition prevents saving if there's a further checkpoint already activated
			if (app->scene->hearts.activated == false)
			{
				lifes++;
				app->scene->hearts.activated = true;
				app->col->DeleteCollider(app->scene->hearts.collider);
			}

		}

	}
}