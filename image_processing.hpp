/*
 * Matthew Diamond 2015
 * Header file for the basic signal processing functions and
 * some callback function helpers.
 */

#ifndef synth_image_processing_h
#define synth_image_processing_h

// Included libraries
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

int open_window();
void populate_samples(std::vector<float> *);
void update_surface(std::vector<float> *);

#endif