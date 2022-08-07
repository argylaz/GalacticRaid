#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "interface.h"

State state;

void update_and_draw() {
    // Initialise KeyState
    KeyState keys = malloc(sizeof(KeyState));
    keys->up =    IsKeyDown(KEY_UP);
    keys->down =  IsKeyDown(KEY_DOWN);
    keys->left =  IsKeyDown(KEY_LEFT);
    keys->right = IsKeyDown(KEY_RIGHT);
    keys->space = IsKeyDown(KEY_SPACE);
    keys->enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
    keys->n =     IsKeyDown(KEY_N);
    keys->p =     IsKeyPressed(KEY_P);

    // Update and draw
	state_update(state, keys);
	interface_draw_frame(state);
}

int main(){
	state = state_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();
    state_destroy(state);

	return 0;
}
