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
#include "FadeToBlack.h"

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

	loadingAnim.PushBack({ 0, 0, 370, 46 });
	loadingAnim.PushBack({ 0, 47, 370, 46 });
	loadingAnim.PushBack({ 0, 94, 370, 46 });
	loadingAnim.PushBack({ 0, 141, 370, 46 });
	loadingAnim.PushBack({ 0, 188, 370, 46 });
	loadingAnim.PushBack({ 0, 235, 370, 46 });
	loadingAnim.speed = 0.00f;

	rDeadAnim.PushBack({ 223, 70, 22, 29 });
	rDeadAnim.PushBack({ 255, 68, 25, 29 });
	rDeadAnim.PushBack({ 287, 79, 26, 19 });
	rDeadAnim.PushBack({ 319, 88, 29, 11 });
	rDeadAnim.PushBack({ 351, 87, 35, 12 });
	rDeadAnim.PushBack({ 339, 86, 25, 13 });
	rDeadAnim.PushBack({ 422, 84, 25, 15 });
	rDeadAnim.PushBack({ 447, 83, 11, 17 });
	rDeadAnim.speed = 0.0f;
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

	texture = app->tex->Load("Assets/Player/player.png");
	heart = app->tex->Load("Assets/Textures/heart.png");
	textureHurt.texture = app->tex->Load("Assets/Player/player_hurt.png");
	textureHurt.loaded = false;
	currentAnimation = &rIdleAnim;

	position.x = 3 * 32; //app->map->data.tileWidth;
	position.y = 94 * 32; //app->map->data.tileHeight;
	/*app->render->camera.x = 0;
	app->render->camera.y = 0;*/
	//app->render->camera.y = -77.5 * app->map->data.tileHeight;

	destroyed = false;

	collider = app->col->AddCollider({ (int)position.x, (int)position.y, 50, 64 }, COLLIDER_TYPE::COLLIDER_PLAYER, this);
	char lookupTable[] = { "01234567/ " };
	scoreFont = app->fonts->Load("Assets/Fonts/score_font.png", lookupTable, 1);
	return ret;
}
void Player::Death() {

	if (deadScreen.loaded == false)
	{
		deadScreen.texture = app->tex->Load("Assets/Screens/dead.png");
		deadScreen.alpha = 255;
		deadScreen.loaded = true;
		position.x = 0;
		position.y = 0;
		rDeadAnim.speed = 0.04f;
	}

	else
	{
		if (app->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_REPEAT)
		{
			reload = true;

			if (map != 1) ChangeMap(1);
			else
			{
				spawnY = 95;
				if (changingSavedMap == false)
				{
					position.x = 3 * 32;
					position.y = spawnY * 32;
					app->render->camera.x = 0;
					app->render->camera.y = -77.5 * app->map->data.tileHeight;
				}
				currentAnimation = &rIdleAnim;
				groundCol = true;
				firstFrame = true;
				spacePressed = false;
				doubleJump = false;
				speedY = 1.6f;

				app->scene->checkPoints[0].position = { 76, 88 };
				if (app->scene->checkPoints[0].activated == true)
				{
					app->scene->checkPoints[0].activated = false;
					app->scene->checkPoints[0].collider = app->col->AddCollider({ app->scene->checkPoints[0].position.x * app->map->data.tileWidth, app->scene->checkPoints[0].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);
				}

				app->scene->checkPoints[1].position = { 33, 44 };
				if (app->scene->checkPoints[1].activated == true)
				{
					app->scene->checkPoints[1].activated = false;
					app->scene->checkPoints[1].collider = app->col->AddCollider({ app->scene->checkPoints[1].position.x * app->map->data.tileWidth, app->scene->checkPoints[1].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);
				}

				app->scene->coins[0].position = { 3, 84 };
				app->scene->coins[0].activated = false;
				app->scene->coins[1].position = { 43, 88 };
				app->scene->coins[1].activated = false;
				app->scene->coins[2].position = { 85, 95 };
				app->scene->coins[2].activated = false;
				app->scene->coins[3].position = { 86, 81 };
				app->scene->coins[3].activated = false;
				app->scene->coins[4].position = { 97, 62 };
				app->scene->coins[4].activated = false;
				app->scene->coins[5].position = { 68, 50 };
				app->scene->coins[5].activated = false;
				app->scene->coins[6].position = { 16, 39 };
				app->scene->coins[6].activated = false;

				app->scene->hearts.position = { 85, 74 };
				if (app->scene->hearts.activated == true)
				{
					app->scene->hearts.activated = false;
					app->scene->hearts.collider = app->col->AddCollider({ app->scene->hearts.position.x * app->map->data.tileWidth, app->scene->hearts.position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, this);
				}

				score = 0;
			}
		}

		if (reload == true)
		{
			app->tex->UnLoad(deadScreen.texture);
			deadScreen.loaded = false;
			rDeadAnim.speed = 0.0f;
			lifes = 3;
			reload = false;
		}

		if (deadScreen.loaded == true)
		{
			SDL_SetTextureAlphaMod(deadScreen.texture, deadScreen.alpha);
			app->render->DrawTexture(deadScreen.texture, app->render->camera.x * -1, app->render->camera.y * -1, NULL, 1);

			app->render->DrawTexture(texture, app->render->camera.x * -1 + 565, app->render->camera.y * -1 + 280, &rDeadAnim.GetCurrentFrame(), 5);
		}
	}
}
bool Player::Update(float dt)
{
	if (lifes == 0)
		Death();

	bool ret = true;
	if (position.x > app->map->data.tileWidth * app->map->data.width || position.x < 0) {
		lifes = 0;
	}

	if (position.y>app->map->data.height*app->map->data.tileHeight) {
		lifes = 0;
	}

	delta = dt;

	// Change speeds with dt
	speedX = dt * speedMultiplier;
	if (speedX - (int)speedX >= 0.5f) speedX = (int)speedX + 1;
	else speedX = (int)speedX;

	if (speedY == 1.6f)
	{
		speedY = 1.6f * dt * speedMultiplier;
		maxNegativeSpeedY = (speedY * -1) + (0.05f * dt * speedMultiplier);
		gravity = (speedY / 160) * dt * speedMultiplier;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		// We could delete the equal frame condition to make the animation more accuret, but it would be less smooth
		if ((currentAnimation != &lJumpAnim && currentAnimation != &rJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.lastFrame - 1]) && groundCol == true)) {
			currentAnimation = &lWalkAnim;
		}

		else if (currentAnimation == &rJumpAnim)
		{
			float frame = currentAnimation->currentFrame;
			currentAnimation = &lJumpAnim;
			currentAnimation->currentFrame = frame;
		}

		else if (currentAnimation == &rFallAnim)
		{
			float frame = currentAnimation->currentFrame;
			currentAnimation = &lFallAnim;
			currentAnimation->currentFrame = frame;
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
		if ((currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim && currentAnimation != &rFallAnim && currentAnimation != &lFallAnim) || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.lastFrame - 1]) && groundCol == true))
		{
			currentAnimation = &rWalkAnim;
		}

		else if (currentAnimation == &lJumpAnim)
		{
			float frame = currentAnimation->currentFrame;
			currentAnimation = &rJumpAnim;
			currentAnimation->currentFrame = frame;
		}

		else if (currentAnimation == &lFallAnim)
		{
			float frame = currentAnimation->currentFrame;
			currentAnimation = &rFallAnim;
			currentAnimation->currentFrame = frame;
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
			speedY = 1.45f * dt * speedMultiplier;
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

		if (speedY < 0)app->render->camera.y += mov - 1;
	}

	if ((position.y + app->render->camera.y) < app->win->screenSurface->h / 4 && collider->type != COLLIDER_GODMODE)
	{
		if (speedY > 0)app->render->camera.y += speedY + 1;
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
		app->render->DrawTexture(texture, position.x, position.y, &rect, 2);
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
				app->render->DrawTexture(textureHurt.texture, position.x, position.y, &textureHurt.rect, 2);
			}
		}

		// Lifes texture rendering
		for (int i = 0, j = 0; i < lifes; ++i, j += 30)
		{
			app->render->DrawTexture(heart, (app->render->camera.x * -1) + 1180 - (30 * lifes) + j, app->render->camera.y * -1 + 25, NULL, 1);
		}

		// Level 1 to level 2 transition
		if (changingMap == true || (loadingScreen.loaded == true && loadingBalls.loaded == true))
		{
			if (loadingScreen.loaded == false)
			{
				loadingScreen.texture = app->tex->Load("Assets/Screens/transition.png");
				loadingScreen.alpha = 255;
				loadingScreen.loaded = true;
				mapChanged = false;
				alphaModifier = 0;
			}

			if (loadingBalls.loaded == false)
			{
				loadingBalls.texture = app->tex->Load("Assets/Screens/loading.png");
				loadingBalls.alpha = 255;
				loadingBalls.loaded = true;
				loadingAnim.Reset();
				loadingAnim.speed = 0.008f;
			}

			if (loadingScreen.loaded == true && loadingScreen.alpha <= 0)
			{
				app->tex->UnLoad(loadingScreen.texture);
				loadingScreen.loaded = false;
			}

			if (loadingBalls.loaded == true && loadingBalls.alpha <= 0)
			{
				app->tex->UnLoad(loadingBalls.texture);
				loadingBalls.loaded = false;
			}

			if (loadingScreen.loaded == true && loadingBalls.loaded == true && alphaModifier == 0)
			{
				SDL_SetTextureAlphaMod(loadingScreen.texture, loadingScreen.alpha);
				app->render->DrawTexture(loadingScreen.texture, app->render->camera.x * -1, app->render->camera.y * -1, NULL, 1);

				SDL_SetTextureAlphaMod(loadingBalls.texture, loadingScreen.alpha);
				app->render->DrawTexture(loadingBalls.texture, app->render->camera.x * -1 + 450, app->render->camera.y * -1 + 500, &loadingAnim.GetCurrentFrame(), 1);

				app->render->DrawTexture(texture, app->render->camera.x * -1 + 575, app->render->camera.y * -1 + 285, &rWalkAnim.GetCurrentFrame(), 5);
			}

			SDL_Rect middle = { 0, 94, 370, 46 };

			// Load second level when in middle of loading animation. mapChanged makes it so that we only call ChangeMap function once
			if (EqualFrames(loadingAnim.GetCurrentFrame(), middle) && mapChanged == false)
				ChangeMap(2);

			SDL_Rect last = { 0, 235, 370, 46 };
			
			// When loading animation is at last frame we start taking alpha and stop the animation
			if (EqualFrames(loadingAnim.GetCurrentFrame(), last))
			{
				int modifier = 3 * (60 / (1 / delta));
				if (modifier > 3 * 1.5f) modifier = 2 * 3;
				else if (modifier > 3 * 0.5f) modifier = 1 * 3;
				alphaModifier = -modifier;
				//mapChanged = false;
				loadingAnim.speed = 0.0f;
			}

			if (loadingScreen.loaded == true) 
				loadingScreen.alpha += alphaModifier;

			if (loadingBalls.loaded == true) 
				loadingBalls.alpha += alphaModifier;

			
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
	int externMap = map;
	map = mapNum;

	if(externMap != map || currentMap != map)
	{
		app->map->Disable();
		app->map->Enable();

		app->col->Disable();
		app->col->Enable();
	}
	//app->map->data.tilesets.start->data->texture = app->tex->Load("");

	switch (map)
	{
	case 1:
		if (externMap != map || currentMap != map)
		{
			app->map->Load("devmap.tmx");
			app->tex->UnLoad(app->scene->img);
			app->scene->img = app->tex->Load("Assets/Textures/background.png");
		}
		spawnY = 94;
		if (changingSavedMap == false)
		{
			position.x = 3 * 32;
			position.y = spawnY * 32;
			app->render->camera.x = 0;
			app->render->camera.y = -77.5 * app->map->data.tileHeight;
		}
		currentAnimation = &rIdleAnim;
		groundCol = true;
		firstFrame = true;
		spacePressed = false;
		doubleJump = false;
		speedY = 1.6f;

		app->scene->checkPoints[0].position = { 76, 88 };
		app->scene->checkPoints[0].activated = false;
		app->col->DeleteCollider(app->scene->checkPoints[0].collider);
		app->scene->checkPoints[0].collider = app->col->AddCollider({ app->scene->checkPoints[0].position.x * app->map->data.tileWidth, app->scene->checkPoints[0].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);
		app->scene->checkPoints[1].position = { 33, 44 };
		app->scene->checkPoints[1].activated = false;
		app->col->DeleteCollider(app->scene->checkPoints[1].collider);
		app->scene->checkPoints[1].collider = app->col->AddCollider({ app->scene->checkPoints[1].position.x * app->map->data.tileWidth, app->scene->checkPoints[1].position.y * app->map->data.tileHeight + 16, app->map->data.tileWidth + 10, app->map->data.tileHeight + 16 }, COLLIDER_TYPE::COLLIDER_CHECKPOINT, this);

		app->scene->coins[0].position = { 3, 84 };
		app->scene->coins[0].activated = false;
		app->scene->coins[1].position = { 43, 88 };
		app->scene->coins[1].activated = false;
		app->scene->coins[2].position = { 85, 95 };
		app->scene->coins[2].activated = false;
		app->scene->coins[3].position = { 86, 81 };
		app->scene->coins[3].activated = false;
		app->scene->coins[4].position = { 97, 62 };
		app->scene->coins[4].activated = false;
		app->scene->coins[5].position = { 68, 50 };
		app->scene->coins[5].activated = false;
		app->scene->coins[6].position = { 16, 39 };
		app->scene->coins[6].activated = false;

		if (externMap != map || currentMap != map)
		{
			for (int i = 0; i < maxScore; i++)
				app->scene->coins[i].collider = app->col->AddCollider({ app->scene->coins[i].position.x * app->map->data.tileWidth, app->scene->coins[i].position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_COIN, this);
		}

		app->scene->hearts.position = { 85, 74 };
		app->scene->hearts.activated = false;
		app->col->DeleteCollider(app->scene->hearts.collider);
		app->scene->hearts.collider = app->col->AddCollider({ app->scene->hearts.position.x * app->map->data.tileWidth, app->scene->hearts.position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, this);

		score = 0;
		break;

	case 2:
		if (externMap != map || currentMap != map)
		{
			app->map->Load("devmap2.tmx");
			app->tex->UnLoad(app->scene->img);
			app->scene->img = app->tex->Load("Assets/Textures/background2.png");
		}

		spawnY = 95;
		if (changingSavedMap == false)
		{
			position.x = 3 * 32;
			position.y = spawnY * 32;
			app->render->camera.x = 0;
			app->render->camera.y = -77.5 * app->map->data.tileHeight;
		}
		currentAnimation = &rIdleAnim;
		groundCol = true;
		firstFrame = true;
		spacePressed = false;
		doubleJump = false;
		speedY = 1.6f;

		app->scene->checkPoints[0].position = {81, 44};
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
		app->col->DeleteCollider(app->scene->hearts.collider);
		app->scene->hearts.collider = app->col->AddCollider({ app->scene->hearts.position.x * app->map->data.tileWidth, app->scene->hearts.position.y * app->map->data.tileHeight, app->map->data.tileWidth, app->map->data.tileHeight }, COLLIDER_TYPE::COLLIDER_HEART, this);
		
		score = 0;
		break;

	default:
		break;
	}

	mapChanged = true;
	changingMap = false;
}

bool Player::Save(pugi::xml_node& data)const
{
	pugi::xml_node player = data.append_child("player");
	pugi::xml_node mapNum = data.append_child("map");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	mapNum.append_attribute("num") = map;

	return true;
}

bool Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_float();

	currentMap = map;

	map = data.child("map").attribute("num").as_int();

	if (currentMap != map)
	{
		changingSavedMap = true;
		mapChanged = false;
		ChangeMap(map);
		changingSavedMap = false;
	}

	return true;
}

bool Player::CheckTunneling(SDL_Rect r1, SDL_Rect r2)
{
	//return (r1.y + speedY < r2.y && r1.y + speedY + r1.h < r2.y + r2.h);
	//return (r1.y + r1.h > r2.y&& r1.y < r2.y);
	int offset = -2 - (120 / (1 / delta));
	if (offset < -9) offset = -9;
	return (r2.y - (r1.h + r1.y) >= offset);
}
void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (collider->type != COLLIDER_GODMODE)
	{
		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && (c2->type == COLLIDER_TYPE::COLLIDER_DEAD || (c2->type == COLLIDER_TYPE::COLLIDER_ENEMY && (c1->rect.y + c1->rect.w / 2 > c2->rect.y))))
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

				// Avoids jumping animation when respawning and sets idle animation when touching a platform
				else if (firstFrame == false && groundCol == true && currentAnimation != &lWalkAnim && currentAnimation != &lIdleAnim && currentAnimation != &lFallAnim && currentAnimation != &lJumpAnim)
					currentAnimation = &rIdleAnim;
				// Avoids jumping animation when respawning and sets idle animation when touching a platform
				else if (firstFrame == false && groundCol == true && currentAnimation != &rWalkAnim && currentAnimation != &rIdleAnim && currentAnimation != &rFallAnim && currentAnimation != &rJumpAnim)
					currentAnimation = &lIdleAnim;

				groundCol = true;
				spacePressed = false;
				doubleJump = false;
				speedY = 1.6f;
			}
		}

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_WIN)
			changingMap = true;

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
					if (app->scene->saveTex.loaded == false) app->scene->saveTex.texture = app->tex->Load("Assets/textures/save_load.png");
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

		if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FINISH)
		{
			app->fade->Fade(this, (Module*)app->winScene, 90);
		}
	}
}