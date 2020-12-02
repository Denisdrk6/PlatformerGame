#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Scene.h"

struct SDL_Texture;
struct Collider;

class Player : public Module
{
public:
	// Constructor
	Player();

	// Destructor
	virtual ~Player();

	bool Awake();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	bool CleanUp();

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;

	//Check if two animation frames are equal
	bool EqualFrames(SDL_Rect, SDL_Rect);

	void ChangeMap(int map);

public:
	// Position of the player in the map
	fPoint			position;

	// The speed in which we move the player (pixels per frame)
	float			speedX = 1;
	float			speedY = 1.45f;
	float			gravity = 0.01f;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture*	texture = nullptr;
	SDL_Texture*	heart = nullptr;
	BlendedTexture	textureHurt;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation*		currentAnimation = nullptr;

	// A set of animations
	Animation		rIdleAnim;
	Animation		lIdleAnim;
	Animation		rWalkAnim;
	Animation		lWalkAnim;
	Animation		rJumpAnim;
	Animation		lJumpAnim;
	Animation		rFallAnim;
	Animation		lFallAnim;

	bool			wallCol = false;
	bool			downCol = false;
	bool			groundCol = true;
	bool			spacePressed = false;
	bool			doubleJump = false;
	bool			firstFrame = true;
	bool			waiting = false;

	int				lifes = 3;
	int				lifeWait = 0;
	bool			lifeTaken = false;
	
	int				spawnX = 95;
	int				map = 1;

	// The player's collider
	Collider*		collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool			destroyed = false;

	// Font score index
	uint			score = 000;
	int				scoreFont = -1;
	char			scoreText[10] = { "\0" };
};

#endif //!__MODULE_PLAYER_H__
