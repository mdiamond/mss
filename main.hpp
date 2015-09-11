/*
 * Matthew Diamond 2015
 * Header file for the driver function.
 */

#ifndef synth_main_h
#define synth_main_h

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"

// Included classes
#include "Module.hpp"

// Audio information
extern int SAMPLE_RATE;
extern int BUFFER_SIZE;

extern int CURRENT_SAMPLE;
extern int AUDIO_LENGTH;

// Graphics stuff
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// The modules currently in use
extern std::vector<Module *> modules;

void audio_callback(void *, Uint8 *, int);

#endif