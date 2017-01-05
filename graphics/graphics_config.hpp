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

/**********************
 * EXTERNAL VARIABLES *
 **********************/

extern TTF_Font *FONT;
extern bool SHOW_TYPING_CURSOR;

// Initialize the graphics library, requires a font to use
void initialize_graphics_library(TTF_Font *);

// Change what font the graphics library will use
void set_graphics_library_font(TTF_Font *);
// Toggle the state of the typing cursor (shown or not shown)
void toggle_show_typing_cursor();

#endif

