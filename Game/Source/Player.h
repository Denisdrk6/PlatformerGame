#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SceneGameplay.h"
#include "Particles.h"

struct SDL_Texture;
struct Collider;

class BlendedTextures
{
public:
	SDL_Texture* texture;
	int alpha;
	bool loaded = false;
	SDL_Rect rect;
};

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

	bool Save(pugi::xml_node&)const;
	bool Load(pugi::xml_node&);

	//Check if two animation frames are equal
	bool EqualFrames(SDL_Rect, SDL_Rect);

	void ChangeMap(int map);
	bool CheckTunneling(SDL_Rect r1, SDL_Rect r2);
	void Death();

	void DrawPointer();

public:
	// Position of the player in the map
	iPoint position;

	// The speed in which we move the player (pixels per frame)
	float speedX = 1.5f;
	float speedY = 1.6f;
	float savedSpeed;
	float gravity = 0.01f;
	float maxNegativeSpeedY;
	float delta = 0.0f;
	int speedMultiplier = 150;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Texture* heart = nullptr;
	SDL_Texture* shot = nullptr;
	BlendedTextures loadingScreen;
	BlendedTextures loadingBalls;
	BlendedTextures textureHurt;
	BlendedTextures deadScreen;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation rIdleAnim;
	Animation lIdleAnim;
	Animation rWalkAnim;
	Animation lWalkAnim;
	Animation rJumpAnim;
	Animation lJumpAnim;
	Animation rFallAnim;
	Animation lFallAnim;
	Animation rShootAnim;
	Animation lShootAnim;
	Animation rDeadAnim;

	Animation loadingAnim;


	bool wallCol = false;
	bool downCol = false;
	bool groundCol = true;
	bool spacePressed = false;
	bool doubleJump = false;
	bool firstFrame = true;
	bool waiting = false;
	bool changingMap = false;
	bool changingSavedMap = false;
	bool mapChanged = false;
	bool reload = false;
	

	int lifes = 3;
	int	lifeWait = 0;
	bool lifeTaken = false;
	
	int	spawnY = 94;
	int	map = 1;
	int	alphaModifier = 0;
	int	alphaModifierDead = 0;

	int currentMap = 1;

	// The player's collider
	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// Font score index
	uint score = 0;
	int scoreFont;
	char scoreText[10] = { "\0" };
	int	maxScore = 7;

	//Audio fx
	int hurtFx = 0;
	int coinsFx = 0;
	int deadFx = 0;
	int shootFx = 0;
	int heartsFx = 0;
	int jumpFx = 0;

	List<Particle*> bullets;
	SDL_Texture* weaponPointer;
	
};

#endif //!__MODULE_PLAYER_H__
