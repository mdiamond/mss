/*
 * Matthew Diamond 2015
 * Header file for the basic image processing functions
 * and some helpers for main(). This file includes any
 * files or libraries necessary for image_processing.cpp
 * and declares any functions.
 */

#ifndef synth_image_processing_h
#define synth_image_processing_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <queue>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Module.hpp"
#include "Oscillator.hpp"
#include "Output.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Initialization functions
int open_window();
int create_renderer();

// Image processing functions
void populate_samples(std::vector<float> *, std::vector<float> *);
void draw_surface();

#endif