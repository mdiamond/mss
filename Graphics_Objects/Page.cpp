/*
 * Matthew Diamond 2015
 * Member functions for the Page class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Page.hpp"

using namespace std;

/*************************
 * PAGE MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Page::Page(SDL_Rect *_location, SDL_Color *_color, vector<Graphics_Object *> *_graphics_objects)
{
    type = PAGE;
    location = *_location;
    color = *_color;

    if(_graphics_objects != NULL && _graphics_objects->size() > 0)
        graphics_objects = new vector<Graphics_Object *>(*_graphics_objects);
}

/*
 * Dummy function.
 */
Page::~Page()
{

}

/*
 * Render the graphics objects.
 */
void Page::render_graphics_object()
{
    for(unsigned int i = 0; i < graphics_objects->size(); i ++)
    {
        (*graphics_objects)[i]->render_graphics_object();
    }
}
