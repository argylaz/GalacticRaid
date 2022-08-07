#include "state_galactic.h"

typedef enum {
    SONG1, SONG2, SONG3
} Song;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);

// Drawing and menu
void menu_draw(State state);

// Drawing about page
void about_draw(State state);

// Drawing settings page
void settings_draw(State state);