/*
 * Matthew Diamond 2015
 * Header file for the basic image processing functions
 * and some helpers for main(). This file declares any functions.
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
int load_fonts();

// Image processing functions
Module *hovering_over();
void reset_alphas();
void draw_surface();

#endif
