/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
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
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"

// Included classes
#include "Module.hpp"
#include "Timer.hpp"

using namespace std;

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Audio information
int SAMPLE_RATE = 44100;
int BUFFER_SIZE;

// SDL Window and renderer
SDL_Window *WINDOW;
SDL_Renderer *RENDERER;

// Module dimensions and amount of modules per page
int MODULE_WIDTH = 150;
int MODULE_HEIGHT = 225;
int MODULES_PER_ROW = 6;
int MODULES_PER_COLUMN = 3;
int MODULE_SPACING = 1;
int MODULE_BORDER_WIDTH = 2;

// Window dimensions
int MENU_HEIGHT = 13;
int WINDOW_WIDTH = (MODULES_PER_ROW * MODULE_WIDTH) +
                   (MODULE_SPACING * MODULES_PER_ROW);
int WINDOW_HEIGHT = (MODULES_PER_COLUMN * MODULE_HEIGHT) +
                    (MODULE_SPACING * MODULES_PER_COLUMN) + MENU_HEIGHT;

// Frames per second and ms per frame
Uint32 FPS = 30;
Uint32 MSPF = 1000 / FPS;

// Fonts
TTF_Font *FONT_REGULAR;
TTF_Font *FONT_BOLD;

// Colors
SDL_Color BLACK = {0, 0, 0 , 255};
SDL_Color WHITE = {255, 255, 255, 255};

// The modules currently in use and whether or not
// The set of modules has been changed recently
vector<Module *> MODULES;
bool MODULES_CHANGED = true;

/***********************
 * TESTING MODE TOGGLE *
 ***********************/

bool testing = false;

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Run in testing mode. If all tests pass, return 1.
 * Otherwise, return 0;
 */
bool testing_mode()
{
    return run_tests();
}

/*
 * Destroy stuff and shut down SDL.
 */
void cleanup()
{
    // Destroy the graphics objects
    SDL_DestroyWindow(WINDOW);
    SDL_DestroyRenderer(RENDERER);

    // Quit SDL
    cout << "Quitting SDL." << endl;
    SDL_Quit();
}

/*
 * Initialize all of the stuff that needs to be initialized
 * before audio can be processed:
 *   - initialize SDL
 *   - open the audio device
 *   - open a window
 *   - create a renderer
 *   - initialize SDL_ttf
 *   - open ttf fonts
 *   - initialize the synthesizer output module
 *   - start audio to begin requesting buffers from the audio
 *     callback function
 * Return true if all of this succeeds, false if anything fails.
 */
bool initialize()
{
    system("clear");

    cout << "Initializing SDL." << endl;
    // Initialize SDL with the video and audio subsystems
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) { 
        cout << "Could not initialize SDL: " << SDL_GetError() << endl;
        return false;
    }
    cout << "SDL initialized." << endl;

    // Initialize audio device
    if(!open_audio_device())
        return false;

    // Open a window
    if(!open_window())
        return false;

    // Create a renderer
    if(!create_renderer())
        return false;

    // Initialize truetype
    if(TTF_Init() == -1)
    {
        cout << "Coult not initialize TTF: " << TTF_GetError() << endl;
        return false;
    }

    // Open ttf fonts
    if(!load_fonts())
        return false;

    // Initialize the output module
    initialize_output();

    // Unpause the audio
    cout << "Unpausing audio." << endl;
    SDL_PauseAudio(0);
    cout << "Audio unpaused." << endl;

    return true;
}

/*
 * Handle events. Return true if SDL_QUIT event
 * is received.
 */
bool event_handler(SDL_Event *e)
{
    bool quit = false;
    while(SDL_PollEvent(e) != 0)
    {
        if(e->type == SDL_QUIT)
        {
            quit = true;
            break;
        }
    }
    return quit;
}

/*
 * Run in normal mode. If all objects create successfully,
 * return true. Otherwise, return false.
 */
bool normal_mode()
{
    if(!initialize())
    {
        cleanup();
        return false;
    }

    // While the user has not quit, continually draw
    // to the window, then delay until the next frame is needed.
    Uint32 frame = 0;
    Uint32 frame_success = 0;
    Uint32 frame_time = 0;
    Uint32 delay_time = 0;
    Uint32 frame_previous = 0;
    Timer *frame_timer = new Timer();
    frame_timer->start();
    SDL_Event e;
    while(true)
    {
        // Calculate the time in ms at which this frame is
        // supposed to be displayed
        frame_time = MSPF * frame;

        // As long as the time that the frame is supposed to be displayed
        // is in the past, move on to the next frame
        while((frame_time = MSPF * frame) < SDL_GetTicks())
            frame ++;

        // If the frame is supposed to be rendered at some point
        // in the future, calculate how many ms until then and delay
        // for that many ms
        if(frame_time > SDL_GetTicks())
        {
            delay_time = frame_time - SDL_GetTicks();
            SDL_Delay(delay_time);
        }

        // Draw the surface
        draw_surface();
        if(event_handler(&e))
        {
            cleanup();
            break;
        }

        // Every 100 frames, print out the framerate
        if(frame % 100 == 0)
        {
            cout << ((frame_success - frame_previous) / (frame_timer->check_time_elapsed() / 1000.0)) << " frames per second." << endl;
            frame_previous = frame_success;
        }

        // Move on to the next frame
        frame ++;
        frame_success ++;
    }

    cleanup();

    return true;
}

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
    cout << "Quitting..." << endl;
    return exit_status;
}
