/*
 * Matthew Diamond 2015
 * This file contains helper functions for the driver function.
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
#include "event_handler.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "populate_wavetables.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"

// Included classes
#include "Color_Modifier.hpp"
#include "Module.hpp"
#include "Timer.hpp"

using namespace std;

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

void destroy_pages()
{
    for(unsigned int i = 0; i < PAGES.size(); i ++)
        delete PAGES[i];
}

void destroy_modules()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        delete MODULES[i];
}

/*
 * Destroy stuff and shut down SDL.
 */
void cleanup()
{
    // Destroy the modules
    destroy_modules();
    cout << "Destroyed all modules." << endl;

    // Destroy all pages
    destroy_pages();
    cout << "Destroyed all pages." << endl;

    // Destroy the window
    SDL_DestroyWindow(WINDOW);
    cout << "Destroyed window." << endl;

    // Destroy the renderer
    SDL_DestroyRenderer(RENDERER);
    cout << "Destroyed renderer." << endl;

    // Quit SDL
    SDL_Quit();
    cout << "SDL terminated." << endl;

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
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1)) { 
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

    // Populate wavetables
    populate_wavetables();

    // Initialize the output module
    initialize_output();

    // Unpause the audio
    cout << "Unpausing audio." << endl;
    SDL_PauseAudio(0);
    cout << "Audio unpaused." << endl;

    return true;
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

    SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND);

    // While the user has not quit, continually draw
    // to the window, then delay until the next frame is needed.
    Uint32 frame = 0;
    Uint32 frame_success = 0;
    Uint32 frame_time = 0;
    Uint32 delay_time = 0;
    Uint32 frame_previous = 0;
    Timer *frame_timer = new Timer();
    SDL_TimerID k_rate_timer = SDL_AddTimer(100, k_rate_callback_function, NULL);
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
            break;

        // Every 100 frames, print out the framerate
        if(frame % 100 == 0)
        {
            cout << GREEN_STDOUT << ((frame_success - frame_previous) / (frame_timer->check_time_elapsed() / 1000.0))
                 << " frames per second." << DEFAULT_STDOUT << endl;
            frame_previous = frame_success;
        }

        // Every 30 frames, negate the status of the typing cursor
        if(frame % 30 == 0)
            CURSOR_ON = !CURSOR_ON;

        // Move on to the next frame
        frame ++;
        frame_success ++;
    }

    // Pause audio
    SDL_PauseAudio(1);

    // Delay to ensure all callback functions finish
    SDL_Delay(100);

    // Clean up
    cleanup();

    // Return success
    return true;
}
