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
Page::Page(std::string name_, SDL_Rect location_, SDL_Color *color_,
           std::vector<Graphics_Object *> *graphics_objects_) :
    Graphics_Object(name_, PAGE, NULL, location_, color_)
{
    if(graphics_objects_ != NULL)
        graphics_objects = std::vector<Graphics_Object *>(*graphics_objects_);
    else
        graphics_objects = std::vector<Graphics_Object *>();
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
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        if(graphics_objects[i] != NULL)
            graphics_objects[i]->render();
}

/*
 * Send the click to all the contained graphics objects.
 */
void Page::clicked()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        if(graphics_objects[i]->was_clicked())
            graphics_objects[i]->clicked();
}

