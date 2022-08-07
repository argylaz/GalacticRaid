#pragma once

#include "raylib.h"
#include "ADTList.h"
#include "set_utils.h"
#include "ADTMap.h"

#define BRIDGE_NUM 20		// Number of bridges per level
#define SPACING 200			// Distance between the objects of the level
#define SCREEN_WIDTH 450	// Width of the screen (in pixels)
#define SCREEN_HEIGHT 800	// Height of the screen (in picels)


typedef enum {
	// Object types
	TERRAIN, HELICOPTER, WARSHIP, JET, BRIDGE, ORB,

	// "Ability" types
	MISSILE,                // Default ability
	LASER,                  // A huge laser with a range of 1 SCREEN_HEIGHT that destroyw anything it touches and moves with the jet
	POWERBLAST,             // A powerfull circular blast that destroys anything in its path(travels until it gets out of the screen)
	SHIELD                  // Blocks one hit that the jet takes. Jet shortly becomes invinsible afterwards jet can't shoot missile while shielded
} ObjectType;

// Types of states that the program can be in
typedef enum{
	MENU, GAME, ABOUT, SETTINGS
} StateType;

// Information for each object
typedef struct object {
	ObjectType type;				// Type (Terain / Helicopter / Warship / Missile / Bridge etc.)
	Rectangle rect;					// Position and size of object (Rectangle declaration is in raylib.h)
	bool forward;					// true: Object is moving to the right of the screen
	int timer;                      // Some objects only exist for a short period (while timer > 0), time is counted in frames
}* Object;

// General info for the current condition of the game
typedef struct state_info {
	Object jet;						// Info about the jet
	Object ability;					// info about the
	Object LastBridge;              // Keeping current last bridge so we can add objects when needed
	Object shield;                  // != NULL when shield is equiped or jet is in invinsibility state
	ObjectType SFX;                 // Used to activate sound effects for missiles and other abilities
	bool orb;                       // indicates that an orb has been collected(non deault ability)
	bool playing;					// true when the game is active (false == game over)
	bool paused;					// true when the game is paused
	bool JustLost;                  // True only on the frame that player lost (used for game-over sound effect)
	bool GameStart;                 // True only on the frame that the game just started (used for music initialisation)
	bool explosion;                 // True only on the frame that the missile destroyed an enemy (used for explosion sound effect)
	int score;						// Current score
	int OrbSpawnRate;               // Chance of orb spawining between two bridges (MOD of rand function)
}* StateInfo;

// The complete info of the game's current state

struct state {
	StateType type;         // The programs current type of state (game, menu etc.)
	Set objects;			// Contains objects
	struct state_info info;	// General info of the game's current state
	float speed_factor;		// All movement in the game is multiplied by this factor
	Map minX;               // Keys = objects, Values = min x coordinate that the object can move
	Map maxX;               // Keys = objects, Values = max x coordinate that the object can move
};

// Info about which keys are pressed in the game's current state
typedef struct key_state {
	bool up;						// true when the equivalent key is pressed
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool m;
	bool p;
}* KeyState;

// Game's current state
typedef struct state* State;


// Creates and returns the starting state of the game

State state_create();

// Returns the basic info of the game's current state

StateInfo state_info(State state);

// Returns a list with all the objects of the game whose y coordinate
// is between y_from and y_to

List state_objects(State state, float y_from, float y_to);

// Updates the game's current state by one frame
// The variable keys contains the keys that were pressed during that frame

void state_update(State state, KeyState keys);

// Destroy's the current state, freeing all allocated memory

void state_destroy(State state);

// Resets state by returning it to it's starting values (Game restart)

void state_reset(State state);