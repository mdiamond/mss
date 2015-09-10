/*
 * Matthew Diamond 2015
 * Header file for the driver function.
 */

#ifndef synth_main_h
#define synth_main_h

// Included libraries
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included classes
#include "Module.hpp"

// Audio information
extern int SAMPLE_RATE;
extern int BUFFER_SIZE;

extern int CURRENT_SAMPLE;
extern int AUDIO_LENGTH;

// Graphics stuff
extern SDL_Window *WINDOW;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// The modules currently in use
extern std::vector<Module *> modules;

void audio_callback(void *, Uint8 *, int);

#endif