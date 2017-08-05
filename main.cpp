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
#include "initialize.hpp"
#include "main.hpp"
#include "main_helpers.hpp"
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
const unsigned int SAMPLE_RATE = 44100;
unsigned int BUFFER_SIZE;
unsigned int NUM_CHANNELS;
bool AUDIO_ON = true;

// Wavetables
std::vector<std::vector<float> > WAVETABLES(4, std::vector<float>(SAMPLE_RATE,
                                                                  0));
// Module dimensions and amount of modules per page
const int MODULE_WIDTH = 160;
const int MODULE_HEIGHT = 135;
const int MODULES_PER_ROW = 6;
const int MODULES_PER_COLUMN = 5;
const int MODULES_PER_PAGE = MODULES_PER_ROW * MODULES_PER_COLUMN;
const int MODULE_SPACING = 2;

// Window dimensions
const int MENU_HEIGHT = 9 + MODULE_SPACING;
const int WINDOW_WIDTH = (MODULES_PER_ROW * MODULE_WIDTH) +
                         ((MODULES_PER_ROW - 1) * MODULE_SPACING);
const int WINDOW_HEIGHT = (MODULES_PER_COLUMN * MODULE_HEIGHT) +
                          (MODULES_PER_COLUMN * MODULE_SPACING) +
                          (MENU_HEIGHT * 2);
SDL_Rect WINDOW_RECT = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

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

// Whether or not the user is currently selecting a source module the toggle
// button for the associated parameter, and the module for which input is
// currently being selected
bool SELECTING_SRC = false;
Toggle_Button *CURRENT_TOGGLE_BUTTON = nullptr;
Module *SELECTING_FOR_MODULE = nullptr;

// Whether or not to use black and white for everything
bool COLORBLIND_ON = false;

// Function forwader to handle interactions with graphics objects that don't
// belong to any module
Function_Forwarder FORWARDER;
Graphics_Listener *NO_MODULE_LISTENER = (Graphics_Listener *) &FORWARDER;

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
        {
            exit_status = -1;
        }
    }

    // If this is normal mode, open SDL, initialize necessary
    // objects, and begin processing audio and graphics
    else
    {
        if(!normal_mode())
        {
            exit_status = -1;
        }
    }

    // Return an exit status based on whether or not
    // the function called above succeeded
    std::cout << "Terminating..." << std::endl;
    return exit_status;
}

