/*
 * Matthew Diamond 2015
 * Header file for the basic signal processing functions and
 * some callback function helpers.
 */

#ifndef synth_image_processing_h
#define synth_image_processing_h

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

int open_window();
int create_renderer();
void populate_samples(std::vector<float> *, std::vector<float> *);
void draw_surface();

#endif