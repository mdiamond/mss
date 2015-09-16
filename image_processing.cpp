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
#include "Oscillator.hpp"
#include "Output.hpp"

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
    FONT_REGULAR = TTF_OpenFont("SourceCodePro-Regular.ttf", 11);
    FONT_BOLD = TTF_OpenFont("SourceCodePro-Bold.ttf", 11);

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
            MODULES[i]->calculate_upper_left(i);
        }
        MODULES_CHANGED = 0;
    }

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->render_border();
        MODULES[i]->render_inner_border();
        MODULES[i]->render();
    }
    SDL_RenderPresent(RENDERER);
}

void render_waveform(SDL_Rect *waveform_location, vector<float> *buffer)
{
    SDL_Point zero = {0, 0};
    vector<SDL_Point> points(waveform_location->w, zero);
    SDL_Rect rect = {waveform_location->x, waveform_location->y, waveform_location->w, waveform_location->h};

    int index = 0;
    for(unsigned int i = buffer->size() - waveform_location->w; i < buffer->size(); i ++)
    {
        points[index].x = waveform_location->x + index;
        points[index].y = (waveform_location->y + waveform_location->h / 2) +
                          ((*(buffer))[buffer->size() - waveform_location->w + index]) * (waveform_location->h / 2);
        index ++;
    }

    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(RENDERER, &rect);

    SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}
