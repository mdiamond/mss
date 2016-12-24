/*
 * Matthew Diamond 2015
 * Member functions for the Rect class.
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
#include "Graphics_Objects/Rect.hpp"

/*************************
 * RECT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Rect::Rect(std::string name_, SDL_Rect location_, SDL_Color color_,
           Graphics_Listener *listener_) :
    Graphics_Object(name_, RECT, listener_, location_, color_)
{
    fill = true;
}

/*
 * Destructor.
 */
Rect::~Rect()
{}

/*
 * Render the rectangle. If fill is set, fill it, otherwise, just draw the
 * outline.
 */
void Rect::render()
{
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                           color.a);
    if(fill)
    {
        SDL_RenderFillRect(RENDERER, &location);
    }
    else
    {
        SDL_RenderDrawRect(RENDERER, &location);
    }
}

