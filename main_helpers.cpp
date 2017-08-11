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
#include "graphics_config.hpp"
#include "event_handler.hpp"
#include "image_processing.hpp"
#include "initialize.hpp"
#include "main.hpp"
#include "tests.hpp"

// Included modules classes
#include "Module.hpp"

// Included "other" classes
#include "Color_Modifier.hpp"
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
    {
        delete PAGES[i];
    }
}

/*
 * Destroy all modules.
 */
void destroy_modules()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            delete MODULES[i];
        }
    }
}

/*
 * Destroy stuff and shut down SDL.
 */
void cleanup()
{
    // Pause audio
    SDL_PauseAudio(1);

    // Destroy the renderer
    if(RENDERER != nullptr)
    {
        SDL_DestroyRenderer(RENDERER);
        std::cout << "Destroyed renderer" << std::endl;
    }

    // Destroy the window
    if(WINDOW != nullptr)
    {
        SDL_DestroyWindow(WINDOW);
        std::cout << "Destroyed window" << std::endl;
    }

    // Destroy the modules
    if(!MODULES.empty())
    {
        destroy_modules();
        std::cout << "Destroyed all modules" << std::endl;
    }

    // Destroy all pages
    if(!PAGES.empty())
    {
        destroy_pages();
        std::cout << "Destroyed all pages" << std::endl;
    }

    // Quit SDL
    SDL_Quit();
    std::cout << "SDL terminated" << std::endl;

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
    unsigned int frame_count = 0;
    Timer timer;
    // SDL_TimerID k_rate_timer = SDL_AddTimer(100, k_rate_callback_function, NULL);
    timer.start();
    SDL_Event e;
    while(true)
    {
        // Handle any events
        if(event_handler(&e))
        {
            break;
        }

        // Draw the surface
        draw_surface();
        frame_count ++;

        // Every 500 frames, print out the framerate
        if(frame_count % 500 == 0)
        {
            std::cout << GREEN_STDOUT << ((frame_count) /
                                          (timer.check_time_elapsed() / 1000.0))
                      << " frames per second." << DEFAULT_STDOUT << std::endl;
            frame_count = 0;
        }

        // Every 30 frames, negate the status of the typing cursor
        if(frame_count % 25 == 0)
        {
            toggle_show_typing_cursor();
        }
    }

    // Clean up
    cleanup();

    // Return success
    return true;
}

