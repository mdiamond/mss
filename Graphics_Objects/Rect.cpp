/*
 * Matthew Diamond 2015
 * Member functions for the Rect class.
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
#include "Rect.hpp"

using namespace std;

/*************************
 * RECT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Rect::Rect(string *_name, SDL_Rect *_location, SDL_Color *_color)
{
    name = *_name;
    type = RECT;
    location = *_location;
    color = *_color;

    fill = true;
}

/*
 * Destructor.
 */
Rect::~Rect()
{

}

/*
 * Dummy function.
 */
void Rect::update_graphics_object()
{

}

/*
 * Render the rectangle. If fill is set, fill it, otherwise,
 * just draw the outline.
 */
void Rect::render_graphics_object()
{
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    if(fill)
        SDL_RenderFillRect(RENDERER, &location);
    else
        SDL_RenderDrawRect(RENDERER, &location);
}

/*
 * Dummy function.
 */
void Rect::clicked()
{

}
