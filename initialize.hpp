/*
 * Matthew Diamond 2016
 * Header file for initialization functions.
 */

#ifndef SYNTH_INITIALIZE_HPP
#define SYNTH_INITIALIZE_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Main initialization function
bool initialize();

// Graphics related initialization functions
int open_window();
int create_renderer();
int create_texture();
int load_fonts();
void initialize_utilities_page();
void prettify_utilities_page();

// Audio related initialization functions
int open_audio_device();
void initialize_output();

#endif

