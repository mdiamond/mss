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
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Text_Box.hpp"

// Included "other" classes
#include "Color_Modifier.hpp"

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
extern unsigned int SAMPLE_RATE;
extern unsigned int BUFFER_SIZE;
extern unsigned int NUM_CHANNELS;
extern bool AUDIO_ON;

// Wavetables
extern std::vector<std::vector<float> > WAVETABLES;
// Graphics objects and variables
//   SDL Window and Renderer
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
extern SDL_Texture *TEXTURE;
//   Module dimensions and amount of modules per page
extern int MODULE_WIDTH;
extern int MODULE_HEIGHT;
extern int MODULES_PER_ROW;
extern int MODULES_PER_COLUMN;
extern int MODULES_PER_PAGE;
extern int MODULE_SPACING;
//   Window dimensions
extern int MENU_HEIGHT;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern SDL_Rect WINDOW_RECT;
//   Font
extern TTF_Font *FONT;
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
//   Mouse information
extern int MOUSE_X;
extern int MOUSE_Y;
extern bool OBJECT_CLICKED;
//   Typing cursor status
extern bool CURSOR_ON;
//   The currently active text box
extern Text_Box *ACTIVE_TEXT_BOX;
//   Selecting a source module or not
extern bool SELECTING_SRC;
extern Input_Toggle_Button *CURRENT_INPUT_TOGGLE_BUTTON;
//   Colorblind mode toggle
extern bool COLORBLIND_ON;

// The modules currently in use and whether or not
// The set of modules has been changed recently
extern std::vector<Module *> MODULES;
extern bool MODULES_CHANGED;

#endif

