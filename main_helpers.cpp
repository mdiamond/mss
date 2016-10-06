/*
 * Matthew Diamond 2015
 * Helper functions for the driver function.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

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

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Destroy all pages of graphics objects.
 */
void destroy_pages()
{
    for(unsigned int i = 0; i < PAGES.size(); i ++)
        delete PAGES[i];
}

/*
 * Destroy all modules.
 */
void destroy_modules()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] != NULL)
            delete MODULES[i];
}

/*
 * Destroy stuff and shut down SDL.
 */
void cleanup()
{
    // Destroy the renderer
    SDL_DestroyRenderer(RENDERER);
    std::cout << "Destroyed renderer." << std::endl;

    // Destroy the window
    SDL_DestroyWindow(WINDOW);
    std::cout << "Destroyed window." << std::endl;

    // Destroy the modules
    destroy_modules();
    std::cout << "Destroyed all modules." << std::endl;

    // Destroy all pages
    destroy_pages();
    std::cout << "Destroyed all pages." << std::endl;

    // Quit SDL
    SDL_Quit();
    std::cout << "SDL terminated." << std::endl;

}

/*
 * Initialize all of the stuff that needs to be initialized
 * before audio can be processed:
 *   - initialize SDL
 *   - open the audio device
 *   - initialize the utilities sub page
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
    // system("clear");

    // Initialize SDL with the video and audio subsystems
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1)) {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL initialized." << std::endl;

    // Initialize audio device
    if(!open_audio_device())
        return false;

    // Initialize truetype
    if(TTF_Init() == -1)
    {
        std::cout << "Could not initialize TTF: " << TTF_GetError() << std::endl;
        return false;
    }

    // Open ttf fonts
    if(!load_fonts())
        return false;

    // Open a window
    if(!open_window())
        return false;

    // Create a renderer
    if(!create_renderer())
        return false;

    // Initialize utilities sub page
    initialize_utilities_page();

    // Create a texture to render to
    if(!create_texture())
        return false;

    // Populate wavetables
    populate_wavetables();

    // Initialize the output module
    initialize_output();

    // Set colors on the utilities page graphics objects
    prettify_utilities_page();

    // Unpause the audio
    SDL_PauseAudio(0);
    std::cout << "Audio unpaused." << std::endl;

    return true;
}

/*************************
 * MAIN HELPER FUNCTIONS *
 *************************/

/*
 * Run in testing mode. If all tests pass, return 1.
 * Otherwise, return 0;
 */
bool testing_mode()
{
    return run_tests();
}

/*
 * Run in normal mode. If everything goes smoothly, return true,
 * otherwise, return false.
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
    Uint32 frame_previous = 0;
    Timer *frame_timer = new Timer();
    // SDL_TimerID k_rate_timer = SDL_AddTimer(100, k_rate_callback_function, NULL);
    frame_timer->start();
    SDL_Event e;
    while(true)
    {
        // Handle any events
        if(event_handler(&e))
            break;

        // Draw the surface
        draw_surface();
        frame ++;

        // Every 100 frames, print out the framerate
        if(frame % 100 == 0)
        {
            std::cout << GREEN_STDOUT << ((frame - frame_previous) / (frame_timer->check_time_elapsed() / 1000.0))
                 << " frames per second." << DEFAULT_STDOUT << std::endl;
            frame_previous = frame;
        }

        // Every 30 frames, negate the status of the typing cursor
        if(frame % 30 == 0)
            CURSOR_ON = !CURSOR_ON;
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
