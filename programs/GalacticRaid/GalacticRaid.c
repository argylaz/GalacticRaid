#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "interface_galactic.h"

State state;

void update_and_draw() {
    KeyState keys = malloc(sizeof(KeyState));

    // Different function calls based on state type
    switch(state->type){
        case MENU:
            menu_draw(state);
            break;

        case ABOUT:
            about_draw(state);
            break;

        case SETTINGS:
            settings_draw(state);
            break;

        case GAME:
            // Initialise KeyState
            keys->up =    IsKeyDown(KEY_UP);
            keys->down =  IsKeyDown(KEY_DOWN);
            keys->left =  IsKeyDown(KEY_LEFT);
            keys->right = IsKeyDown(KEY_RIGHT);
            keys->space = IsKeyDown(KEY_SPACE);
            keys->enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
            keys->n =     IsKeyDown(KEY_N);
            keys->p =     IsKeyPressed(KEY_P);
            keys->m =     IsKeyPressed(KEY_M);

            // Update state and draw frame
            state_update(state, keys);
            interface_draw_frame(state);
            break;

        default:
            break;
    }
}

int main(){
	state = state_create();
	interface_init();
    state->info.playing = false;
    //state->type = MENU;

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();
    state_destroy(state);
	return 0;
}
