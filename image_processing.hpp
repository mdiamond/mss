/*
 * Matthew Diamond 2015
 * Header file for the mage processing functions and graphics related
 * initialization functions
 */

#ifndef synth_image_processing_h
#define synth_image_processing_h

/************
 * INCLUDES *
 ************/

// Included classes
#include "Module.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Initialization functions
int open_window();
int create_renderer();
int create_texture();
int load_fonts();

// Image processing functions
Module *hovering_over();
void draw_surface();

#endif
