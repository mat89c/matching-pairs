#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *title_font;
    TTF_Font *button_font;
    SDL_Color text_color;
    SDL_Color button_color;
    SDL_Color button_hover_color;
    SDL_Rect start_button;
    SDL_Rect options_button;
    SDL_Rect quit_button;
} MainMenu;

// Initialize the main menu
bool init_main_menu(MainMenu *menu);

// Render the main menu
void render_main_menu(MainMenu *menu);

// Handle menu events
void handle_menu_events(MainMenu *menu, bool *quit);

// Clean up menu resources
void cleanup_main_menu(MainMenu *menu);

// Save current menu state as JPG image
bool save_menu_as_jpg(MainMenu *menu, const char *filename);

#endif // MAIN_MENU_H
