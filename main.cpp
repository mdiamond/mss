/*
 * Matthew Diamond 2015
 * The driver function. Everything starts
 * and ends here. It initializes all external variables,
 * then either runs the testing functions or starts SDL and
 * begins processing.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL.h"

// Included files
#include "event_handler.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "main_helpers.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"

// Included modules classes
#include "Module.hpp"

// Included "other" classes
#include "Timer.hpp"

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Color codes for stdout output
Color_Modifier DEFAULT_STDOUT(Color_Modifier::FG_DEFAULT);
Color_Modifier RED_STDOUT(Color_Modifier::FG_RED);
Color_Modifier GREEN_STDOUT(Color_Modifier::FG_GREEN);
Color_Modifier BLUE_STDOUT(Color_Modifier::FG_BLUE);
Color_Modifier PINK_STDOUT(Color_Modifier::FG_PINK);

// Audio information
int SAMPLE_RATE = 44100;
int BUFFER_SIZE;
bool AUDIO_ON = true;

// Wavetables
std::vector<std::vector<float> > WAVETABLES(4, std::vector<float>(SAMPLE_RATE, 0));

// SDL Window and renderer
SDL_Window *WINDOW;
SDL_Renderer *RENDERER;
SDL_Texture *TEXTURE;

// Module dimensions and amount of modules per page
int MODULE_WIDTH = 150;
int MODULE_HEIGHT = 225;
int MODULES_PER_ROW = 6;
int MODULES_PER_COLUMN = 3;
int MODULES_PER_PAGE = MODULES_PER_ROW * MODULES_PER_COLUMN;
int MODULE_SPACING = 1;

// Window dimensions
int MENU_HEIGHT = 19;
int WINDOW_WIDTH = (MODULES_PER_ROW * MODULE_WIDTH) +
                   ((MODULES_PER_ROW - 1) * MODULE_SPACING);
int WINDOW_HEIGHT = (MODULES_PER_COLUMN * MODULE_HEIGHT) +
                    (MODULES_PER_COLUMN * MODULE_SPACING) +
                    (MENU_HEIGHT * 2) - 3;
SDL_Rect WINDOW_RECT = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

// Fonts
TTF_Font *FONT_REGULAR;
TTF_Font *FONT_SMALL;
TTF_Font *FONT_BOLD;

// Colors
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color RED = {255, 0, 0, 255};
SDL_Color GREEN = {0, 255, 0, 255};
SDL_Color BLUE = {0, 0, 255, 255};

// Pages of graphics objects
std::vector<Page *> PAGES = std::vector<Page *>();
Page *UTILITIES_PAGE;
unsigned int CURRENT_PAGE = 0;

// Mouse information
int MOUSE_X;
int MOUSE_Y;
bool OBJECT_CLICKED = false;

// Typing cursor status
bool CURSOR_ON = true;

// The currently active text box
Text_Box *ACTIVE_TEXT_BOX = NULL;

// Whether or not the user is currently selecting a source module
// and the input toggle button for the associated parameter
bool SELECTING_SRC = false;
Input_Toggle_Button *CURRENT_INPUT_TOGGLE_BUTTON = NULL;

// The modules currently in use and whether or not
// The set of modules has been changed recently
std::vector<Module *> MODULES = std::vector<Module *>();
bool MODULES_CHANGED = true;

/***********************
 * TESTING MODE TOGGLE *
 ***********************/

bool testing = false;

/*****************
 * MAIN FUNCTION *
 *****************/

/*
 * Driver function for the whole program. This program
 * will either run in test mode or normally. If run in test
 * mode this function will run a series of tests and then exit.
 * If run normally, this function will open the audio device,
 * initialize the output module, and then wait for user interaction.
 */
int main()
{
    int exit_status = 0;

    // If this is testing mode, just run the tests
    if(testing)
    {
        if(!testing_mode())
            exit_status = -1;
    }

    // If this is normal mode, open SDL, initialize necessary
    // objects, and begin processing audio and video
    else
    {
        if(!normal_mode())
            exit_status = -1;
    }

    // Return an exit status based on whether or not
    // the function called above succeeded and terminate
    std::cout << "Terminating..." << std::endl;
    return exit_status;
}
