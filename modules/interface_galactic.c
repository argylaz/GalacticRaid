#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_galactic.h"

Texture background_img;
Texture rocket_img;
Texture orb_img;
Texture meteorite_img;
Texture ufo_img;
Texture powerblast_img;
Texture shield_img;
Texture laser_img;
Texture wall_img;
Texture logo_img;
Texture button_img;
Texture left_arrow_button_img;
Texture right_arrow_button_img;
Texture text_img;
Texture start_text_img;
Texture about_text_img;
Texture settings_text_img;
Texture start_icon_img;
Texture about_icon_img;
Texture settings_icon_img;
Texture music_icon_img;
Texture volume_icon_img;
Texture percentage20_img;
Texture percentage25_img;
Texture percentage30_img;
Texture percentage50_img;
Texture percentage100_img;
Texture song1_img;
Texture song2_img;
Texture song3_img;
Texture spawn_rate_text_img;

Sound game_over_snd;
Sound woosh_snd;
Sound explosion_snd;
Sound laser_snd;
Sound powerblast_snd;
Sound missile_snd;

Music game_music;

int volume;
Song song;

void interface_init(){
    // Initialising window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

    InitAudioDevice();

    // Loading textures
    background_img = LoadTextureFromImage(LoadImage("assets/space_background.png"));
    rocket_img = LoadTextureFromImage(LoadImage("assets/rocket.png"));
    orb_img = LoadTextureFromImage(LoadImage("assets/orb.png"));
    powerblast_img = LoadTextureFromImage(LoadImage("assets/powerblast.png"));
    shield_img = LoadTextureFromImage(LoadImage("assets/shield.png"));
    laser_img = LoadTextureFromImage(LoadImage("assets/laser.png"));
    meteorite_img = LoadTextureFromImage(LoadImage("assets/meteorite.png"));
    ufo_img = LoadTextureFromImage(LoadImage("assets/ufo.png"));
    wall_img = LoadTextureFromImage(LoadImage("assets/metal_plate.png"));
    logo_img = LoadTextureFromImage(LoadImage("assets/galactic_raid_logo.png"));
    button_img = LoadTextureFromImage(LoadImage("assets/button.png"));
    left_arrow_button_img = LoadTextureFromImage(LoadImage("assets/left_arrow_button.png"));
    right_arrow_button_img = LoadTextureFromImage(LoadImage("assets/right_arrow_button.png"));
    text_img = LoadTextureFromImage(LoadImage("assets/text.png"));
    start_text_img = LoadTextureFromImage(LoadImage("assets/start_text.png"));
    about_text_img = LoadTextureFromImage(LoadImage("assets/about_text.png"));
    settings_text_img = LoadTextureFromImage(LoadImage("assets/settings_text.png"));
    start_icon_img = LoadTextureFromImage(LoadImage("assets/start_icon.png"));
    about_icon_img = LoadTextureFromImage(LoadImage("assets/about_icon.png"));
    settings_icon_img = LoadTextureFromImage(LoadImage("assets/settings_icon.png"));
    music_icon_img = LoadTextureFromImage(LoadImage("assets/music_icon.png"));
    volume_icon_img = LoadTextureFromImage(LoadImage("assets/volume_icon.png"));
    percentage20_img = LoadTextureFromImage(LoadImage("assets/20%.png"));
    percentage25_img = LoadTextureFromImage(LoadImage("assets/25%.png"));
    percentage30_img = LoadTextureFromImage(LoadImage("assets/30%.png"));
    percentage50_img = LoadTextureFromImage(LoadImage("assets/50%.png"));
    percentage100_img = LoadTextureFromImage(LoadImage("assets/100%.png"));
    song1_img = LoadTextureFromImage(LoadImage("assets/song1.png"));
    song2_img = LoadTextureFromImage(LoadImage("assets/song2.png"));
    song3_img = LoadTextureFromImage(LoadImage("assets/song3.png"));
    spawn_rate_text_img = LoadTextureFromImage(LoadImage("assets/spawn_rate_text.png"));


    // Loading sound effects
    game_over_snd = LoadSound("assets/game_over.mp3");
    woosh_snd = LoadSound("assets/woosh.mp3");
    explosion_snd = LoadSound("assets/explosion.mp3");
    missile_snd = LoadSound("assets/laserblast.mp3");
    laser_snd = LoadSound("assets/laser.mp3");
    powerblast_snd = LoadSound("assets/powerblast.mp3");

    volume = 5;
    song = SONG1;

    // Loading default music
    game_music = LoadMusicStream("assets/song1.mp3");
    SetMusicVolume(game_music, ((float) volume)/ 10);
}

void interface_close() {
    // Loading textures
    UnloadTexture(rocket_img);
    UnloadTexture(orb_img);
    UnloadTexture(powerblast_img);
    UnloadTexture(shield_img);
    UnloadTexture(laser_img);
    UnloadTexture(background_img);
    UnloadTexture(meteorite_img);
    UnloadTexture(ufo_img);
    UnloadTexture(wall_img);
    UnloadTexture(logo_img);
    UnloadTexture(button_img);
    UnloadTexture(right_arrow_button_img);
    UnloadTexture(left_arrow_button_img);
    UnloadTexture(text_img);
    UnloadTexture(start_text_img);
    UnloadTexture(about_text_img);
    UnloadTexture(settings_text_img);
    UnloadTexture(start_icon_img);
    UnloadTexture(about_icon_img);
    UnloadTexture(settings_icon_img);
    UnloadTexture(music_icon_img);
    UnloadTexture(volume_icon_img);
    UnloadTexture(percentage20_img);
    UnloadTexture(percentage25_img);
    UnloadTexture(percentage30_img);
    UnloadTexture(percentage50_img);
    UnloadTexture(percentage100_img);
    UnloadTexture(song1_img);
    UnloadTexture(song2_img);
    UnloadTexture(song3_img);
    UnloadTexture(spawn_rate_text_img);


    // Unloading sounds and music
    UnloadSound(game_over_snd);
    UnloadSound(woosh_snd);
    UnloadSound(explosion_snd);
    UnloadMusicStream(game_music);

    // CloseAudioStream();
    CloseAudioDevice();
	CloseWindow();
}

// Drawing menu
void menu_draw(State state){
    BeginDrawing();

    // Draw backgorund
    DrawTexture(background_img, 0, 0, WHITE);

    // Draw logo
    DrawTexture(logo_img, 25, 100, WHITE);


    //////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////// PROCESS FOR START BUTTON /////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Initialise start button
    Rectangle StartButton;
    StartButton.x = SCREEN_WIDTH/2 - 100;
    StartButton.y = SCREEN_HEIGHT/2 - 200;
    StartButton.width = 200;
    StartButton.height = 76;
    Color C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), StartButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            state->info.playing = true;
            state->type = GAME;
        }
    }
    // Draw start button
    DrawTexture(button_img, StartButton.x, StartButton.y, C);
    DrawTexture(
        start_text_img,
        SCREEN_WIDTH/2 - MeasureText("START", 30)/2 + 25,
        SCREEN_HEIGHT/2 - 180,
        SKYBLUE
    );
    DrawTexture(start_icon_img, StartButton.x + 25, StartButton.y + 20, SKYBLUE);


    //////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// PROCESS FOR ABOUT BUTTON ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Initialise about button
    Rectangle AboutButton;
    AboutButton.x = SCREEN_WIDTH/2 - 100;
    AboutButton.y = SCREEN_HEIGHT/2 - 100;
    AboutButton.width = 200;
    AboutButton.height = 76;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), AboutButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            state->type = ABOUT;
        }
    }
    // Draw about button
    DrawTexture(button_img, AboutButton.x, AboutButton.y, C);
    DrawTexture(
        about_text_img,
        SCREEN_WIDTH/2 - MeasureText("START", 30)/2 + 15,
        SCREEN_HEIGHT/2 - 80,
        SKYBLUE
    );
    DrawTexture(about_icon_img, AboutButton.x + 25, AboutButton.y + 20, SKYBLUE);


    //////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// PROCESS FOR SONG BUTTON ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Initialise about button
    Rectangle SettingsButton;
    SettingsButton.x = SCREEN_WIDTH/2 - 100;
    SettingsButton.y = SCREEN_HEIGHT/2;
    SettingsButton.width = 200;
    SettingsButton.height = 76;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), SettingsButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            state->type = SETTINGS;
        }
    }
    // Drawing settings button
    DrawTexture(button_img, SettingsButton.x, SettingsButton.y, C);
    DrawTexture(
       settings_text_img,
       SCREEN_WIDTH/2 - MeasureText("SETTINGS", 30)/2 + 45,
       SCREEN_HEIGHT/2 + 20 + 3,
       SKYBLUE
    );
    DrawTexture(settings_icon_img, SettingsButton.x + 25, SettingsButton.y + 20, SKYBLUE);


    EndDrawing();
}

// Drawing about page
void about_draw(State state){
    BeginDrawing();

    // Draw backgorund
    DrawTexture(background_img, 0, 0, WHITE);

    // Draw logo
    DrawTexture(logo_img, 25, 100, WHITE);

    // Draw about text
    DrawTexture(text_img, 0, 200, WHITE);

    // Initialise back to menu button
    Rectangle BackButton;
    BackButton.x = SCREEN_WIDTH - 70;
    BackButton.y = 20;
    BackButton.width = 50;
    BackButton.height = 63;
    Color C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), BackButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            state->type = MENU;
        }
    }
    // Draw back to menu button
    DrawTexture(left_arrow_button_img, BackButton.x, BackButton.y, C);

    EndDrawing();
}

// Drawing settings page
void settings_draw(State state){
    BeginDrawing();

    // Filling background with background image
    DrawTexture(background_img, 0, 0, WHITE);

    // Draw logo
    DrawTexture(logo_img, 25, 100, WHITE);

    //////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// PROCESS FOR SONG BUTTON ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Draw song button
    DrawTexture(button_img, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 200, WHITE);
    DrawTexture(music_icon_img, SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 - 180, SKYBLUE);
    Texture SongName; // Displayed on song button
    switch (song){
        case SONG1:
            SongName = song1_img;
            break;
            
        case SONG2:
            SongName = song2_img;
            break;
        
        case SONG3:
            SongName = song3_img;
            break;

        default:
            break;
    }
    DrawTexture(SongName, SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2 - 178, SKYBLUE);

    // Initialise left button
    Rectangle LeftButton;
    LeftButton.x = SCREEN_WIDTH/2 - 160;
    LeftButton.y = SCREEN_HEIGHT/2 - 193;
    LeftButton.width = 50;
    LeftButton.height = 63;
    Color C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), LeftButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Change song when left button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(song > SONG1){
                song--;

                UnloadMusicStream(game_music);
                switch (song){
                    case SONG1:
                        game_music = LoadMusicStream("assets/song1.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;
                    
                    case SONG2:
                        game_music = LoadMusicStream("assets/song2.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;

                    case SONG3:
                        game_music = LoadMusicStream("assets/song3.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;

                    default:
                        break;
                }
            }
        }

    }
    // Draw left song button
    DrawTexture(
        left_arrow_button_img,
        LeftButton.x,
        LeftButton.y,
        C
    );

    // Initialise right button
    Rectangle RightButton;
    RightButton.x = SCREEN_WIDTH/2 + 110;
    RightButton.y = SCREEN_HEIGHT/2 - 193;
    RightButton.width = 50;
    RightButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), RightButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Change song when right button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(song < SONG3){
                song++;

                UnloadMusicStream(game_music);
                switch (song){
                    case SONG1:
                        game_music = LoadMusicStream("assets/song1.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;
                    
                    case SONG2:
                        game_music = LoadMusicStream("assets/song2.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;

                    case SONG3:
                        game_music = LoadMusicStream("assets/song3.mp3");
                        SetMusicVolume(game_music, ((float) volume)/ 10);
                        break;

                    default:
                        break;
                }
            }
        }

    }
    // Draw right button
    DrawTexture(
        right_arrow_button_img,
        RightButton.x,
        RightButton.y,
        C
    );



    //////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// PROCESS FOR VOLUME BUTTON //////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Draw volume button
    DrawTexture(button_img, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, WHITE);
    DrawTexture(volume_icon_img, SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 - 80, SKYBLUE);
    DrawRectangle(
        SCREEN_WIDTH/2 - 40,
        SCREEN_HEIGHT/2 - 75,
        12 * volume,
        20,
        SKYBLUE
    );

    // Initialise left button
    LeftButton.x = SCREEN_WIDTH/2 - 160;
    LeftButton.y = SCREEN_HEIGHT/2 - 93;
    LeftButton.width = 50;
    LeftButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), LeftButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Set music volume lower when button is pressed 
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(volume > 0)
                volume--;
            SetMusicVolume(game_music, ((float) volume) / 10);
        }

    }
    // Draw left button
    DrawTexture(
        left_arrow_button_img,
        LeftButton.x,
        LeftButton.y,
        C
    );

    // Initialise right music button
    RightButton.x = SCREEN_WIDTH/2 + 110;
    RightButton.y = SCREEN_HEIGHT/2 - 93;
    RightButton.width = 50;
    RightButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), RightButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Set music volume higher when button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(volume < 10)
                volume++;
            SetMusicVolume(game_music, ((float) volume) / 10);
        }

    }
    // Draw left music button
    DrawTexture(
        right_arrow_button_img,
        RightButton.x,
        RightButton.y,
        C
    );



    //////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// PROCESS FOR SPAWN RATE BUTTON /////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Draw spawn rate button
    DrawTexture(button_img, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, WHITE);
    Texture percentage; // Different texture depending on spawn rate
    switch (state->info.OrbSpawnRate){
        case 1:
            percentage = percentage100_img;
            break;

        case 2:
            percentage = percentage50_img;
            break;
        
        case 3:
            percentage = percentage30_img;
            break;

        case 4:
            percentage = percentage25_img;
            break;

        case 5:
            percentage = percentage20_img;
            break;
        
        default:
            percentage = percentage20_img;
            break;
    }
    DrawTexture(percentage, SCREEN_WIDTH/2 - 35, SCREEN_HEIGHT/2 + 20, SKYBLUE);
    DrawTexture(spawn_rate_text_img, SCREEN_WIDTH/2 - 110, SCREEN_HEIGHT/2 + 80, SKYBLUE);

    // Initialise left music button
    LeftButton.x = SCREEN_WIDTH/2 - 160;
    LeftButton.y = SCREEN_HEIGHT/2; //- 93;
    LeftButton.width = 50;
    LeftButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), LeftButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(state->info.OrbSpawnRate < 5)
                state->info.OrbSpawnRate++;
            state_reset(state); // State needs to be reset with the new spawn rate
            state->type = SETTINGS;
        }

    }
    // Draw left music button
    DrawTexture(
        left_arrow_button_img,
        LeftButton.x,
        LeftButton.y,
        C
    );

    // Initialise right button
    RightButton.x = SCREEN_WIDTH/2 + 110;
    RightButton.y = SCREEN_HEIGHT/2;// - 93;
    RightButton.width = 50;
    RightButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), RightButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            if(state->info.OrbSpawnRate > 1)
                state->info.OrbSpawnRate--;
            state_reset(state); // State needs to be reset with the new spawn rate
            state->type = SETTINGS;
        }

    }
    // Draw right button
    DrawTexture(
        right_arrow_button_img,
        RightButton.x,
        RightButton.y,
        C
    );


    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// PROCESS FOR BACK TO MENU BUTTON /////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Initialise back to menu button
    Rectangle BackButton;
    BackButton.x = SCREEN_WIDTH - 70;
    BackButton.y = 20;
    BackButton.width = 50;
    BackButton.height = 63;
    C = WHITE;
    if(CheckCollisionPointRec(GetMousePosition(), BackButton)){
        C = GRAY; // Darker colour when cursor is hovering over button

        // Start game when start button is pressed
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(woosh_snd);
            state->type = MENU;
        }
    }
    // Draw back to menu button
    DrawTexture(left_arrow_button_img, BackButton.x, BackButton.y, C);

    EndDrawing();
}

// Draw game (one frame)
void interface_draw_frame(State state) {
    BeginDrawing();

    // If game just started, start playing music
    if(state->info.GameStart){
        PlayMusicStream(game_music);
        state->info.GameStart = false;
    }

    // Play explosion sound effect when needed
    if(state->info.explosion){
        PlaySound(explosion_snd);
        state->info.explosion = false;
    }

    // Play ability sound effect when needed
    switch (state->info.SFX) {
        case MISSILE:
            PlaySound(missile_snd);
            state->info.SFX = JET;
            break;
        
        case LASER:
            PlaySound(laser_snd);
            state->info.SFX = JET;
            break;

        case POWERBLAST:
            PlaySound(powerblast_snd);
            state->info.SFX = JET;
            break;

        default: // In any other case no sound effect is played
            break;
    }

    // Play music (while not paused)
    if(!state->info.paused)
        UpdateMusicStream(game_music);

    StateInfo info = state_info(state);

    // Calculating y-offset based on jet position
    // No need for x-offset since state and window coordinates are the same
    float y_offset = info->jet->rect.y - SCREEN_HEIGHT + 150;

    // Filling background with background image
    DrawTexture(background_img, 0, 0, WHITE);

    // Drawing all objects within the screen (except for terrain)
    List Objects = state_objects(state, info->jet->rect.y - SCREEN_HEIGHT, info->jet->rect.y + 150);
    for(ListNode node = list_first(Objects);
        node != LIST_EOF;
        node = list_next(Objects, node)){

        Object O = list_node_value(Objects, node);

        switch (O->type){
        case BRIDGE:
            DrawRectangle(O->rect.x, O->rect.y - y_offset, O->rect.width, O->rect.height, DARKGRAY);
            break;

        case HELICOPTER:
            DrawTexture(ufo_img, O->rect.x - 20, O->rect.y - y_offset - 35, WHITE);
            break;

        case WARSHIP:
            DrawTexture(meteorite_img, O->rect.x - 3, O->rect.y - y_offset, WHITE);
            break;

        case ORB:
            DrawTexture(orb_img, O->rect.x - 3, O->rect.y - y_offset - 3, WHITE);
            break;

        default:
            break;
        }
    }
    list_destroy(Objects);

    

    // Draw ability if one exists in this state
    if(info->ability != NULL){
        switch(info->ability->type){
            case MISSILE:
                DrawRectangle(
                    info->ability->rect.x,
                    info->ability->rect.y - y_offset,
                    info->ability->rect.width,
                    info->ability->rect.height,
                    RED
                );
                break; 

            case LASER:
                DrawTexture(
                    laser_img,
                    info->ability->rect.x - 17,
                    info->ability->rect.y - y_offset,
                    WHITE
                );
                break;
                
            
            case POWERBLAST:
                // Using orb texture with dark blue colour because it looks cool
                DrawTexture(
                    powerblast_img,
                    info->ability->rect.x,
                    info->ability->rect.y - y_offset,
                    WHITE
                );
                break;
            
            default:
                break;
        }
    }

    // Draw all terrain that exists 2 screen height's away from  the jet
    Objects = state_objects(state, info->jet->rect.y - 2 * SCREEN_HEIGHT, info->jet->rect.y + 150);
    for(ListNode node = list_first(Objects);
        node != LIST_EOF;
        node = list_next(Objects, node)){
        
        Object O = list_node_value(Objects, node);
        if(O->type == TERRAIN)
            DrawTextureRec(wall_img, O->rect, (Vector2){O->rect.x, O->rect.y - y_offset}, WHITE);
    }
    list_destroy(Objects);

    // Drawing jet
    if(info->shield != NULL && info->shield->forward){
        // When in invinsibility state, jet is "flashing"
        if(rand()%20 > 5)
            DrawTexture(
                    rocket_img,
                    info->jet->rect.x - 7,
                    info->jet->rect.y - y_offset - 5,
                    WHITE
            );
    }
    else
        DrawTexture(
            rocket_img,
            info->jet->rect.x - 7,
            info->jet->rect.y - y_offset - 5,
            WHITE
        );

    // Drawing shield after the jet
    if(info->shield != NULL && !info->shield->forward)
        DrawTexture(
            shield_img,
            info->jet->rect.x - 8,
            info->jet->rect.y - y_offset - 5,
            WHITE
        );

    // Draw framerate on the top right corner
    DrawFPS(SCREEN_WIDTH - 73, 0);

    // Draw score on the top left corner
    char *text = malloc(2 * sizeof(char));
    sprintf(text, "%d", info->score);
    DrawText(
        text,
        5,
        0,
        20,
        WHITE
    );
    free(text);

    // Draw indication that an orb has been aquired
    if(state->info.orb)
        DrawTexture(orb_img, 4, info->jet->rect.y - SCREEN_HEIGHT + 175 - y_offset, WHITE);

    // When game is over, draw text to restart
    if (!info->playing) {
        // Play game-over sound when just lost
        if(info->JustLost){
            StopMusicStream(game_music);
            PlaySound(game_over_snd);
            info->JustLost = false;
        }

        // Draw texts
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			 SCREEN_WIDTH / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 SCREEN_HEIGHT / 2 - 10,
             20,
             GRAY
		);

        DrawText(
			"PRESS [M] TO OPEN MENU",
			 SCREEN_WIDTH / 2 - MeasureText("PRESS [M] TO OPEN MENU", 20) / 2,
			 SCREEN_HEIGHT / 2 + 10,
             20,
             GRAY
		);
	}

    EndDrawing();
}