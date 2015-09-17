/*
 * Matthew Diamond 2015
 * This file contains all of the basic image processing functions.
 * It also contains helper functions for main().
 */

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
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

using namespace std;

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
    WINDOW = SDL_CreateWindow("Visualizer",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL
                             );

    if (WINDOW == NULL) {
        cout << "Could not create window: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Window opened." << endl;

    return 1;
}

/*
 * Create a renderer for the window created before.
 */
int create_renderer()
{
    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);

    if (WINDOW == NULL) {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Renderer created." << endl;

    return 1;
}

/*
 * Load fonts into external variables so that all
 * modules can render text.
 */
int load_fonts()
{
    FONT_REGULAR = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 11);
    FONT_BOLD = TTF_OpenFont("fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_BOLD)
    {
        cout << "Could not open one of the TTF fonts: " << TTF_GetError() << endl;
        return 0;
    }

    return 1;
}

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Render the GUI in the window by using the
 * member functions in each module that render
 * their representations
 */
void draw_surface()
{
    // First, lock the audio, then copy all data necessary for graphics
    SDL_LockAudio();

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->copy_graphics_data();
    }

    // Once all data necessary for graphics has been copied,
    // unlock the audio, then if necessary, re-render the
    // module borders, then render all the modules, then
    // present what has been rendered
    SDL_UnlockAudio();

    SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(RENDERER, &background);

    if(MODULES_CHANGED)
    {
        for(unsigned int i = 0; i < MODULES.size(); i ++)
        {
            MODULES[i]->calculate_graphics_objects(i);
        }
        MODULES_CHANGED = 0;
    }

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->update_graphics_objects();
        MODULES[i]->render_module();
    }
    SDL_RenderPresent(RENDERER);
}
