/*
 * Matthew Diamond 2015
 * Functions for constructing batches of graphics objects.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>

// Included files
#include "main.hpp"

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Increment the current page by the amount specified.
 */
void increment_page_number(int num)
{
    if(CURRENT_PAGE + num < PAGES.size())
    {
        CURRENT_PAGE += num;
        std::cout << "Switched to page " << CURRENT_PAGE << std::endl;
    }
}

