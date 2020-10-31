#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
//#include "Particles.h"
#include "Audio.h"
#include "Collisions.h"
//#include "FadeToBlack.h"
//#include "Fonts.h"
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
	rWalkAnim.speed = 0.15f;

	lWalkAnim.PushBack({ 160, 104, 32, 32 });
	lWalkAnim.PushBack({ 128, 104, 32, 32 });
	lWalkAnim.PushBack({ 96, 104, 32, 32 });
	lWalkAnim.PushBack({ 64, 104, 32, 32 });
	lWalkAnim.PushBack({ 32, 104, 32, 32 });
	lWalkAnim.PushBack({ 0, 104, 32, 32 });
	lWalkAnim.speed = 0.15f;

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
	//++activeTextures; ++totalTextures;
	currentAnimation = &rIdleAnim;

	laserFx = app->audio->LoadFx("Assets/Fx/laser.wav");
	//++activeFx; ++totalFx;

	explosionFx = app->audio->LoadFx("Assets/Fx/explosion.wav");
	//++activeFx; ++totalFx;

	position.x = 2 * app->map->data.tileWidth;
	position.y = 89 * app->map->data.tileHeight;

	destroyed = false;

	collider = app->col->AddCollider({ position.x, position.y, 50, 64 }, COLLIDER_TYPE::COLLIDER_PLAYER, this);
	//++activeColliders; ++totalColliders;

	/*char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = app->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);
	++activeFonts; ++totalFonts;*/

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	//TODO 3 fix camera movement with player

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		
		if (currentAnimation != &lJumpAnim && currentAnimation != &rJumpAnim || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1])))
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

		if (position.x > app->win->screenSurface->w / 12)
		{
			app->render->camera.x += speed;
		}

		position.x -= speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		
		if ((currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim) || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1])))
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

		if (position.x > app->win->screenSurface->w / 12)
		{
			app->render->camera.x -= speed;
			
		}
		position.x += speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT && jump == false)
	{
		if (jump == false) jump = true;
		fall = false;
	}

	if (jump == true && fall == false)
	{
		if (currentAnimation != &rJumpAnim && currentAnimation != &lJumpAnim)
		{
			if (currentAnimation == &rWalkAnim || currentAnimation == &rIdleAnim)
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
			app->render->camera.y += speed;
		}
	}

	/*if (app->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		Particle* newParticle = app->particles->AddParticle(app->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(this);
		app->audio->PlayFx(laserFx);
	}*/

	 //If no right/left/up movement detected, set the current animation back to idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_IDLE
		&& (currentAnimation != &rIdleAnim && currentAnimation != &lIdleAnim))
	{
		if (currentAnimation == &rWalkAnim || (currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[rJumpAnim.last_frame - 1])))
		{
			currentAnimation = &rIdleAnim;
			jump = false;
		}
		else if (currentAnimation == &lWalkAnim || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[lJumpAnim.last_frame - 1])))
		{
			currentAnimation = &lIdleAnim;
			jump = false;
		}
	}

	//Gravity
	//TODO 5 no funciona que si está en mitad de la animacion de salto se active la gravedad, hay que hacer que se active si está cayendo
	if ((currentAnimation == &rJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), rJumpAnim.frames[4]) || (currentAnimation == &lJumpAnim && EqualFrames(currentAnimation->GetCurrentFrame(), lJumpAnim.frames[4]))) || fall == true)
	{
		//TODO 4 falta añadir si esta colisionando con el suelo que no lo haga
		fall = true;
		position.y += speed;
	}

	//TODO 1 acabar colisiones
	collider->SetPos(position.x, position.y);

	//currentAnimation->Update();

	//TODO 7 hacer que si el jugador colisiona con suelo, fall se haga false

	//TODO 6 hacer que la camara solo baje si el jugador está cayendo, para esto hace falta poner colisiones del jugador porque cuando se acaba la animacion de salto se pone la de idle, aunque tendria que cambiar a una animacion de caer y cuando tocase el suelo cambiar a idle
	/*if ((currentAnimation == )&& position.y > app->win->screenSurface->w / 4)
	{
		app->render->camera.y -= speed * 2;
	}*/

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

	/*
	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);

	app->fonts->BlitText(58, 248, scoreFont, scoreText);

	app->fonts->BlitText(150, 248, scoreFont, "this is just a font test");*/

	return ret;
}

bool Player::CleanUp()
{
	//activeTextures = activeColliders = activeFonts = activeFx = 0;

	// TODO 1: Remove ALL remaining resources. Update resource count properly

	app->tex->UnLoad(texture);
	//--totalTextures;

	//TODO 2 acabar audios (sonnido salto, etc...)
	/*
	app->audio->UnloadFx(laserFx);
	//--totalFx;

	app->audio->UnloadFx(explosionFx);
	//--totalFx;*/

	/*app->collisions->RemoveCollider(collider);
	--totalColliders;

	app->fonts->UnLoad(scoreFont);
	--totalFonts;*/

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR)
	{
		if(c2->rect.y - (c1->rect.h + c1->rect.y) >= -1)
		fall = false;
	}
}