/*
 * Matthew Diamond 2015
 * Header file for the mage processing functions and graphics related
 * initialization functions
 */

#ifndef SYNTH_IMAGE_PROCESSING_HPP
#define SYNTH_IMAGE_PROCESSING_HPP

/************
 * INCLUDES *
 ************/

// Included modules classes
#include "Module.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Initialization functions
int open_window();
int create_renderer();
int create_texture();
int load_fonts();
void initialize_utilities_page();
void prettify_utilities_page();

// Image processing functions
Module *hovering_over();
void draw_surface();

#endif
