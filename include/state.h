#pragma once

#include "raylib.h"
#include "ADTList.h"

#define BRIDGE_NUM 20		// Number of bridges per level
#define SPACING 200			// Distance between the objects of the level
#define SCREEN_WIDTH 450	// Width of the screen (in pixels)
#define SCREEN_HEIGHT 800	// Height of the screen (in picels)

typedef enum {
	TERRAIN, HELICOPTER, WARSHIP, JET, MISSILE, BRIDGE
} ObjectType;

// Information for each object
typedef struct object {
	ObjectType type;				// Type (Terain / Helicopter / Warship / Missile / Bridge etc.)
	Rectangle rect;					// Position and size of object (Rectangle declaration is in raylib.h)
	bool forward;					// true: Object is moving to the right of the screen
}* Object;

// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
typedef struct state_info {
	Object jet;						// Info about the jet
	Object missile;					// info about the missile
	bool playing;					// true when the game is active (false == game over)
	bool paused;					// true when the game is paused
	int score;						// Current score
}* StateInfo;

// Info about which keys are pressed in the game's current state
typedef struct key_state {
	bool up;						// true when the equivalent key is pressed
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool p;
	bool m;
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