/*
 * Matthew Diamond 2015
 * Header file for the driver function. This file declares
 * all external variables and includes any files or libraries
 * necessary for main.cpp.
 */

#ifndef synth_main_h
#define synth_main_h

/************
 * INCLUDES *
 ************/

// Included SDL components
#include "SDL2/SDL_ttf.h"

// Included classes
#include "Module.hpp"

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Audio information
extern int SAMPLE_RATE;
extern int BUFFER_SIZE;

extern int CURRENT_SAMPLE;
extern int AUDIO_LENGTH;

// Graphics objects and variables
//   SDL Window and Renderer
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
//   Module dimensions and amount of modules per page
extern int MODULE_WIDTH;
extern int MODULE_HEIGHT;
extern int MODULES_PER_ROW;
extern int MODULES_PER_COLUMN;
extern int MODULE_SPACING;
extern int MODULE_BORDER_WIDTH;
//   Window dimensions
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
//   Frames per second and ms per frame
extern Uint32 FPS;
extern Uint32 MSPF;
//   Fonts
extern TTF_Font *FONT_REGULAR;
extern TTF_Font *FONT_BOLD;

// The modules currently in use and whether or not
// The set of modules has been changed recently
extern std::vector<Module *> MODULES;
extern int MODULES_CHANGED;

#endif
