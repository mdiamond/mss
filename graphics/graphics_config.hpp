/*
 * A header file that contains configuration variables and an initialization
 * function for the graphics library. The initialization function must be run
 * before using the graphics library so that it has the necessary resources
 * for rendering available. Also found here are headers for functions for
 * changing aspects of the graphics library configuration.
 */

#ifndef GRAPHICS_GRAPHICS_CONFIG_HPP
#define GRAPHICS_GRAPHICS_CONFIG_HPP

/************
 * INCLUDES *
 ************/

// Included SDL components
#include "SDL_ttf.h"

// Forward declare Text_Box class
class Text_Box;

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Font and typing cursor on/off boolean
extern TTF_Font *FONT;
extern bool SHOW_TYPING_CURSOR;
// SDL Window and Renderer
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
extern SDL_Texture *TEXTURE;
// Mouse information
extern int MOUSE_X;
extern int MOUSE_Y;
// The currently active text box
extern Text_Box *ACTIVE_TEXT_BOX;

// Initialize the graphics library, requires a font to use
void initialize_graphics_library(TTF_Font *);

// Change what font the graphics library will use
void set_graphics_library_font(TTF_Font *);
// Toggle the state of the typing cursor (shown or not shown)
void toggle_show_typing_cursor();

#endif

