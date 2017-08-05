/*
 * Matthew Diamond 2015
 * Header file for the driver function. This file declares
 * all external variables.
 */

#ifndef MSS_MAIN_HPP
#define MSS_MAIN_HPP

/************
 * INCLUDES *
 ************/

// Included libraries
#include <map>

// Included SDL components
#include "SDL_ttf.h"

// Included modules classes
#include "Module.hpp"

// Included graphics classes
#include "Graphics_Listener.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Text_Box.hpp"

// Included "other" classes
#include "Color_Modifier.hpp"
#include "Function_Forwarder.hpp"

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Color codes for stdout output
extern Color_Modifier DEFAULT_STDOUT;
extern Color_Modifier RED_STDOUT;
extern Color_Modifier GREEN_STDOUT;
extern Color_Modifier BLUE_STDOUT;
extern Color_Modifier PINK_STDOUT;

// Audio information
extern const unsigned int SAMPLE_RATE;
extern unsigned int BUFFER_SIZE;
extern unsigned int NUM_CHANNELS;
extern bool AUDIO_ON;

// Wavetables
extern std::vector<std::vector<float> > WAVETABLES;

// Graphics objects and variables
//   Module dimensions and amount of modules per page
extern const int MODULE_WIDTH;
extern const int MODULE_HEIGHT;
extern const int MODULES_PER_ROW;
extern const int MODULES_PER_COLUMN;
extern const int MODULES_PER_PAGE;
extern const int MODULE_SPACING;
//   Window dimensions
extern const int MENU_HEIGHT;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern SDL_Rect WINDOW_RECT;
//   Colors
extern SDL_Color BLACK;
extern SDL_Color WHITE;
extern SDL_Color RED;
extern SDL_Color GREEN;
extern SDL_Color BLUE;
//   Pages of graphics objects
extern std::vector<Page *> PAGES;
extern Page *UTILITIES_PAGE;
extern unsigned int CURRENT_PAGE;
//   Selecting a source module or not
extern bool SELECTING_SRC;
extern Toggle_Button *CURRENT_TOGGLE_BUTTON;
extern Module *SELECTING_FOR_MODULE;
//   Colorblind mode toggle
extern bool COLORBLIND_ON;
//   Function forwarder
extern Function_Forwarder FORWARDER;
extern Graphics_Listener *NO_MODULE_LISTENER;

// The modules currently in use and whether or not
// The set of modules has been changed recently
extern std::vector<Module *> MODULES;
extern bool MODULES_CHANGED;

#endif

