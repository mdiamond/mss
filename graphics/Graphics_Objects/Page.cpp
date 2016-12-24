/*
 * Matthew Diamond 2015
 * Member functions for the Page class.
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

// Included files
#include "main.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Page.hpp"

/*************************
 * PAGE MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Page::Page(std::string name_, SDL_Rect location_, SDL_Color color_,
           std::vector<Graphics_Object *> *graphics_objects_) :
    Graphics_Object(name_, PAGE, NULL, location_, color_)
{
    if(graphics_objects_ != NULL)
    {
        graphics_objects = std::vector<Graphics_Object *>(*graphics_objects_);
    }
    else
    {
        graphics_objects = std::vector<Graphics_Object *>();
    }
}

/*
 * Destructor.
 */
Page::~Page()
{

}

/*
 * Render the graphics objects.
 */
void Page::render()
{
    for(auto itr = graphics_objects.begin(); itr != graphics_objects.end();
        itr ++)
    {
        (*itr)->render();
    }
}

/*
 * Send the click to all the contained graphics objects. Return true if action
 * is taken, false otherwise.
 */
bool Page::clicked()
{
    bool ret = false;
    for(auto itr = graphics_objects.begin(); itr != graphics_objects.end();
        itr ++)
    {
        if((*itr)->mouse_over())
        {
            if((*itr)->clicked())
            {
                ret = true;
            }
        }
    }
    return ret;
}

