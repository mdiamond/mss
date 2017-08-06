/*
 * Matthew Diamond 2016
 * Header file for initialization functions.
 */

#ifndef MSS_INITIALIZE_HPP
#define MSS_INITIALIZE_HPP

/************
 * INCLUDES *
 ************/

// Included SDL components
#include "SDL_ttf.h"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Main initialization function
bool initialize();

// Graphics related initialization functions
bool open_window();
bool create_renderer();
bool create_texture();
bool load_font(TTF_Font **);
void initialize_utilities_page();
void prettify_utilities_page();

// Audio related initialization functions
bool open_audio_output_device();
bool open_audio_input_device();
void initialize_output();

#endif

