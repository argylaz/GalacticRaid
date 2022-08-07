
#include <stdlib.h>

#include "set_utils.h"
#include "state.h"
#include "ADTMap.h"


// Complete info of the game's current state

struct state {
	Set objects;			// Stores all objects (Warships/Helicopters/terrains... etc.)
	struct state_info info;	// General info of the game's current state
	float speed_factor;		// Speed factor (determines the speed of the game)
	Object LastBridge;      // Keeping current last bridge so we can add objects when needed
	Map minX;               // Keys = objects, Values = min x coordinate that the object can move
	Map maxX;               // Keys = objects, Values = max x coordinate that the object can move
};


// Creates a new object
static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Creates a float equal to value inside the heap
float* create_float(float value){
	float *P = malloc(sizeof(float*));
	*P = value;
	return P;
}

// Compares objects based on y coordinate of their rectangle
// From highest y coordinate (0) to lowest 
// Objects are considered equal when they have the same y coordinate and pointer
int compare_obj(Pointer a, Pointer b){
	Object obj1 = a;
	Object obj2 = b;

	// Comparing y coordinates
	if(obj1->rect.y > obj2->rect.y)
		return 1;
	else if(obj1->rect.y < obj2->rect.y)
		return -1;
	else // obj1->rect.y == obj2->rect.y
		return  b - a;
}

// Adds objects to the level (level doesn't need to be empty).
// They start from y_from and go upwards
//
// **NOTE** :  NEGATIVE Y COORDINATES ARE UPWARDS AND POSITIVE DOWNWARDS
//
// We save each objects x/y coordinates along with its height and width
// These values define a rectangle so we can store them all as Rectangles in obj->rect
// which is a pre-defined struct of raylib.h
// The x and y coordinates are the top left corner of the rectangle

static void add_objects(State state, float start_y) {
	// Adding BRIDGE_NUM bridges.
	// Between each pair of bridges we add :
	// - Terrain, left and right of the screen (with variable width).
	// - 3 enemies (warships or helicopters)
	// Each pair of objects are SPACING pixels affar from each other

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x in the left-most point of the screen
			start_y - 4 * (i+1) * SPACING,	// Bridge i has y = 4 * (i+1) * SPACING
			SCREEN_WIDTH,					// As wide as the screen
			20								// Height
		);

		// Update last bridge
		if(i == BRIDGE_NUM - 1)
			state->LastBridge = bridge;

		// Creating terrain
		Object terrain_left = create_object(
			TERRAIN,
			0,								// Left terrain, x = 0
			bridge->rect.y,					// y same as the bridge
			rand() % (SCREEN_WIDTH/3),		// Random width
			4*SPACING						// Height enogh to fill in the space between 2 bridges
		);
		int width = rand() % (SCREEN_WIDTH/3);
		Object terrain_right = create_object(
			TERRAIN,
			SCREEN_WIDTH - width,			// Right terrain, x = <screen> - <terrain width>
			bridge->rect.y,					// y same as the bridge
			width,							// Random width
			4*SPACING						// Height enogh to fill in the space between 2 bridges
		);

		set_insert(state->objects, terrain_left);
		set_insert(state->objects, terrain_right);
		set_insert(state->objects, bridge);

		// Adding 3 enemies before each bridge
		for (int j = 0; j < 3; j++) {
			// With SPACING pixels between each other
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Randomly pick a helicopter or a warship
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// Centered horizontally
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Starting with a random moving direction

			set_insert(state->objects, enemy);
		}
	}
}

// Creates and returns the starting condition of the game
State state_create() {
	// Cerating state
	State state = malloc(sizeof(*state));

	// General ingo
	state->info.playing = true;				// Game starts
	state->info.paused = false;				// Not Paysed
	state->info.score = 0;					// Starting score = 0
	state->info.missile = NULL;				// No missile at the start
	state->speed_factor = 1;				// Normal speed

	// Creating jet, centered horizontally with y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	// Creating a set of objects and adding them starting from y_from = 0
	state->objects = set_create(compare_obj, free);
	add_objects(state, 0);

	// Creating maps
	// Only destroying values, not Keys(objects)
	// because they exist in other data structures(state->objects)
	state->maxX = map_create(compare_obj, NULL, free);
	state->minX = map_create(compare_obj, NULL, free);

	return state;
}

// Resets state values (restarting game at the menu) 
void state_reset(State state){
	// This function is basically the same a state_create 
	// But frees resets previous values instead of creating new state
	if(state->info.missile != NULL)
		free(state->info.missile);
	free(state->info.jet);
		
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	state->info.playing = true;				// Game starts
	state->info.paused = false;				// Not paused
	state->info.score = 0;					// Starting score = 0
	state->info.missile = NULL;				// No missile at the start
	state->speed_factor = 1;				// Default speed

	map_destroy(state->maxX);
	state->maxX = map_create(compare_obj, NULL, free);
	
	map_destroy(state->minX);
	state->minX = map_create(compare_obj, NULL, free);

	set_destroy(state->objects);
	state->objects = set_create(compare_obj, free);
	add_objects(state, 0);
}

// Returns the current info of the game 
StateInfo state_info(State state) {
	return &(state->info);
}

// Returns a list with all the objects in the current state
// with y coordinate between y_from and y_to
List state_objects(State state, float y_from, float y_to) {
	// Set will hold all objects with y coordinate >= y_from and <= y_to
	List Objects = list_create(NULL);

	// Creating a random object with y_from to insert and find the next one in the set
	// IF set_find_eq_or_greater returns null the loop must start from SET_BOF
	// else we need to find the object in the set
	Object tempO = set_find_eq_or_greater(state->objects, create_object(1, 1, y_from, 1, 1));
	SetNode node_from;
	if(tempO != NULL)
		node_from = set_find_node(state->objects, tempO);
	else
		node_from = set_first(state->objects);

	// Find the first node >= y_to to know when to terminate the for loop
	// and save it so we dont have to find it each time
	SetNode tempN = set_find_node(state->objects, set_find_eq_or_smaller(state->objects, create_object(1, 1, y_to, 1, 1)));
	SetNode node_to = set_next(state->objects, tempN);

	// Add all set objects from y_from to y_to to the new set
	for(SetNode node = node_from;
		node != node_to;
		node = set_next(state->objects, node)){

			// Find node value and insert in new set
			Object v = set_node_value(state->objects, node);
			list_insert_next(Objects, list_last(Objects), v);
	}

	return Objects;
}

// Updates the state of the game 1 frame
// keys = keys pressed during this frame
void state_update(State state, KeyState keys) {
	StateInfo info = state_info(state);

	// Game-over state logic
	if(info->playing == false){
		if(keys->enter){
			state_reset(state); // Reset game
		}
		else return;
	}

	// Pause logic
	if(keys->p)
		info->paused = !info->paused;
	if(info->paused && !keys->n) // Returning while N is not pressed
		return;

	// Missile launching and movement
	if(info->missile == NULL){
		if(keys->space)
			info->missile = create_object(MISSILE, info->jet->rect.x + info->jet->rect.width/2, info->jet->rect.y - 15, 5, 15);
	}
	else{ // A missile already exists
		info->missile->rect.y -= 10 * state->speed_factor;
		
		// If missile has traveled too far, destroy it
		if(info->missile->rect.y - info->jet->rect.y < -SCREEN_HEIGHT){
			free(info->missile);
			info->missile = NULL;
		}
	}
	

	// Changing jet's y coordinate based on keys pressed
	if(keys->up)
		info->jet->rect.y -= 6 * state->speed_factor;
	else if (keys->down)
		info->jet->rect.y -= 2 * state->speed_factor;
	else
		info->jet->rect.y -= 3 * state->speed_factor;

	// Changing jet's x coordinate based on keys pressed
	if(keys->left) 
		info->jet->rect.x += -3 * state->speed_factor;
	else if(keys->right) 
		info->jet->rect.x += 3 * state->speed_factor;

	// Find all objects that are maximum 2 screens away from the jet
	List Objects = state_objects(state, info->jet->rect.y - 2 * SCREEN_HEIGHT, info->jet->rect.y + 150);

	// Used to know whether a bridge has been previously found in the list
	// so that we only update the enemies that collide with
	// terrain that exists in the list
	bool FoundBridge = false;

	// Repeating process for every object found
	for(ListNode node = list_first(Objects);
		node != LIST_EOF;
		node = list_next(Objects, node)){

		Object O = list_node_value(Objects, node);

		// Check whether a bridge has been found
		if(O->type == BRIDGE)
			FoundBridge = true;

		// Check enemy collisions (from map or find value to insert in map)
		// Only if a bridge has been found
		if((O->type == HELICOPTER || O->type == WARSHIP) && FoundBridge){
			float *Xmin = map_find(state->minX, O);
			float *Xmax = map_find(state->maxX, O);

			// Check if limits for O are known (exists as a key in the map)
			// If not try to find the limits to insert them
			if(Xmin != NULL && O->rect.x <= *Xmin)
				O->forward = true;
			else if(Xmax != NULL && O->rect.x >= *Xmax)
				O->forward = false;
			else if(Xmin == NULL || Xmax == NULL)
				// Process to find minX and maxX (only if enemy collides with terrain)
				for(ListNode node2 = list_first(Objects);
					node2 != LIST_EOF;
					node2 = list_next(Objects, node2)){

					Object O2 = list_node_value(Objects, node2);
					
					// When enemy collides with terrain, insert limit to map
					if(O2->type == TERRAIN && CheckCollisionRecs(O->rect, O2->rect)){
						if(O->forward)
							map_insert(state->maxX, O, create_float(O2->rect.x - O->rect.width));
						else // Object's direction of movement is left
							map_insert(state->minX, O, create_float(O2->rect.x + O2->rect.width));
					}
				}
		}

		// Changing enemy's x coordinate
		// Only if a bridge has been found
		if(FoundBridge){
			if(O->type == WARSHIP)
				O->rect.x += ((O->forward) ? 3 : -3) * state->speed_factor;
			else if(O->type == HELICOPTER)
				O->rect.x += ((O->forward) ? 4 : -4) * state->speed_factor;
		}

		// Checking for jet collision with any object
		if(CheckCollisionRecs(info->jet->rect, O->rect)){
			info->playing = false;  // Game-over if jet crashed
			return;
		}

		// Check for missile collision (if one exists)
		if(info->missile != NULL){
			if(CheckCollisionRecs(info->missile->rect, O->rect)){
				free(info->missile);
				info->missile = NULL;

				// If object is not terrain then its Helicopter/Warship/Bridge
				// Because they are the only other types on the list
				if(O->type != TERRAIN){
					state->info.score += 10;

					// Find previous node in list to remove object from it
					SetNode prevSet = set_previous(state->objects, set_find_node(state->objects, O));
					ListNode prevList = list_find_node(Objects, set_node_value(state->objects, prevSet), compare_obj);
					list_remove_next(Objects, prevList);

					// Remove object from set
					set_remove(state->objects, O);

					//This is needed for the for loop to continue properly
					node = prevList;

					continue;
				}
			}
		}
	}

	// Destroy list
	list_destroy(Objects);

	// Make level seemingly infinite by adding more objects when close to the last bridge
	if(state->LastBridge->rect.y - info->jet->rect.y > -SCREEN_HEIGHT){
		add_objects(state, state->LastBridge->rect.y);

		// Make the game 30% faster
		state->speed_factor += (30 * state->speed_factor)/100;
	}

	return;
}


// Destroy's the current state, freeing the allocated memory
void state_destroy(State state) {
	map_destroy(state->minX);
	map_destroy(state->maxX);
	set_destroy(state->objects);
	free(state->info.jet);
	if(state->info.missile != NULL)
		free(state->info.missile);
	free(state);
	return;
}