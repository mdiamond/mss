/*
 * Matthew Diamond 2016
 * Initialization and configuration functions for the graphics library.
 */

/************
 * INCLUDES *
 ************/

// Included files
#include "graphics_config.hpp"

TTF_Font *FONT = nullptr;
bool SHOW_TYPING_CURSOR = false;

SDL_Window *WINDOW = nullptr;
SDL_Renderer *RENDERER = nullptr;
SDL_Texture *TEXTURE = nullptr;

int MOUSE_X = 0;
int MOUSE_Y = 0;

Text_Box *ACTIVE_TEXT_BOX = nullptr;

/*
 * Initialize the graphics library, requires a font to use.
 */
void initialize_graphics_library(TTF_Font *font)
{
    set_graphics_library_font(font);
}

/*
 * Change what font the graphics library will use.
 */
void set_graphics_library_font(TTF_Font *font)
{
    FONT = font;
}

/*
 * Toggle the state of the typing cursor (shown or not shown).
 */
void toggle_show_typing_cursor()
{
    SHOW_TYPING_CURSOR = !SHOW_TYPING_CURSOR;
}

