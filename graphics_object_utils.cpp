/*
 * Matthew Diamond 2015
 * Functions for constructing batches of graphics objects.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Increment the current page by the amount specified.
 */
void increment_page_number(int num)
{
    unsigned int tmp = CURRENT_PAGE;

    tmp += num;

    if(tmp >= 0 && tmp < PAGES.size())
    {
        CURRENT_PAGE = tmp;
        std::cout << "Switched to page " << CURRENT_PAGE << std::endl;
    }
}

