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

// No includes necessary

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Initialization functions
int open_window();
int create_renderer();
int load_fonts();

// Image processing functions
void draw_surface();

#endif
