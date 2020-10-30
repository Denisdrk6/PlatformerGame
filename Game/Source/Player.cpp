#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
//#include "Particles.h"
#include "Audio.h"
//#include "Collisions.h"
//#include "FadeToBlack.h"
//#include "Fonts.h"
#include "Log.h"
#include "Map.h"
#include "Window.h"

#include <stdio.h>

Player::Player() : Module()
{
	name.Create("player");

	// idle animation - just one sprite
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
	position.y = 11.5 * app->map->data.tileHeight;

	destroyed = false;

	//collider = app->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);
	//++activeColliders; ++totalColliders;

	/*char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = app->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);
	++activeFonts; ++totalFonts;*/

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	// Moving the player with the camera scroll
	//app->player->position.x += 1;

	//WTF3 fix camera movement with player

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		currentAnimation = &lWalkAnim;
		if (position.x > app->win->screenSurface->w / 12)
		{
			app->render->camera.x += speed;
		}

		position.x -= speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		currentAnimation = &rWalkAnim;
		if (position.x > app->win->screenSurface->w / 12)
		{
			app->render->camera.x -= speed;
		}
		position.x += speed;
	}

	/*if (app->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		Particle* newParticle = app->particles->AddParticle(app->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(this);
		app->audio->PlayFx(laserFx);
	}*/

	 //If no right/left movement detected, set the current animation back to idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& (currentAnimation != &rIdleAnim && currentAnimation != &lIdleAnim))
	{
		if (currentAnimation == &rWalkAnim) 
			currentAnimation = &rIdleAnim;
		else if (currentAnimation == &lWalkAnim) 
			currentAnimation = &lIdleAnim;
	}
		

	//collider->SetPos(position.x, position.y);

	//WTF
	//currentAnimation->Update();

	return ret;
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

	//WTF2
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

/*void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		App->audio->PlayFx(explosionFx);
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneIntro, 60);

		destroyed = true;
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 23;
	}
}*/