#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

Image warship;
Image helicopter;

Texture jet_img;
Texture warship_img;
Texture warship_reversed_img;
Texture helicopter_img;
Texture helicopter_reversed_img;

void interface_init(){
    // Initialising window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

    // Loading images and textures
    warship = LoadImage("assets/warship.png");
    helicopter = LoadImage("assets/helicopter.png");
    jet_img = LoadTextureFromImage(LoadImage("assets/jet.png"));

    warship_reversed_img = LoadTextureFromImage(LoadImage("assets/warship.png"));
    ImageFlipHorizontal(&warship);
    warship_img = LoadTextureFromImage(warship);

    helicopter_img = LoadTextureFromImage(LoadImage("assets/helicopter.png"));
    ImageFlipHorizontal(&helicopter);
    helicopter_reversed_img = LoadTextureFromImage(helicopter);
}

void interface_close() {
    UnloadTexture(jet_img);

    UnloadImage(warship);
    UnloadTexture(warship_img);
    UnloadTexture(warship_reversed_img);

    UnloadImage(helicopter);
    UnloadTexture(helicopter_img);
    UnloadTexture(helicopter_reversed_img);

	CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state) {
    BeginDrawing();

    StateInfo info = state_info(state);

    // Calculating y-offset based on jet position
    // No need for x-offset since state and window coordinates are the same
    float y_offset = info->jet->rect.y - SCREEN_HEIGHT + 150;

    // Filling background with blue(sea)
    ClearBackground(BLUE);

    // Drawing jet
    DrawTexture(
            jet_img,
            info->jet->rect.x + 2,
            info->jet->rect.y - y_offset,
            WHITE
    );

    // Drawing all objects within the screen (except for terrain)
    List Objects = state_objects(state, info->jet->rect.y - SCREEN_HEIGHT, info->jet->rect.y + 150);
    for(ListNode node = list_first(Objects);
        node != LIST_EOF;
        node = list_next(Objects, node)){

        Object O = list_node_value(Objects, node);

        switch (O->type){
        case BRIDGE:
            DrawRectangle(O->rect.x, O->rect.y - y_offset, O->rect.width, O->rect.height, RED);
            break;

        case HELICOPTER:
            if(O->forward)
                DrawTexture(helicopter_img, O->rect.x, O->rect.y - y_offset - 10, YELLOW);
            else
                DrawTexture(helicopter_reversed_img, O->rect.x, O->rect.y - y_offset - 10, YELLOW);
            break;

        case WARSHIP:
            if(O->forward)
                DrawTexture(warship_img, O->rect.x, O->rect.y - y_offset - 25, MAROON);
            else
                DrawTexture(warship_reversed_img, O->rect.x, O->rect.y - y_offset - 25, MAROON);
            break;

        default:
            break;
        }
    }
    list_destroy(Objects);

    // Draw all terrain that exists 2 screen height's away from  the jet
    Objects = state_objects(state, info->jet->rect.y - 2 * SCREEN_HEIGHT, info->jet->rect.y + 150);
    for(ListNode node = list_first(Objects);
        node != LIST_EOF;
        node = list_next(Objects, node)){
        
        Object O = list_node_value(Objects, node);
        if(O->type == TERRAIN)
            DrawRectangle(O->rect.x, O->rect.y - y_offset, O->rect.width, O->rect.height, DARKGREEN);
    }
    list_destroy(Objects);

    // Draw framerate on the top right corner
    char *text = malloc(6 * sizeof(char));
    sprintf(text, "%d FPS", GetFPS());
    DrawText(
        text,
        SCREEN_WIDTH - MeasureText(text, 20) -5,
        0,
        20,
        GREEN
    );
    free(text);

    // Draw score on the top left corner
    text = malloc(2 * sizeof(char));
    sprintf(text, "%d", info->score);
    DrawText(
        text,
        5,
        0,
        20,
        WHITE
    );
    free(text);

    // Draw missile if one exists
    if(info->missile != NULL){
        DrawRectangle(
                info->missile->rect.x,
                info->missile->rect.y - y_offset,
                info->missile->rect.width,
                info->missile->rect.height,
                WHITE
        );
    }

    // When game is over, draw text to restart
    if (!info->playing) {
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			 SCREEN_WIDTH / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 SCREEN_HEIGHT / 2, 20, GRAY
		);
	}

    EndDrawing();
}