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
#include "function_forwarder.hpp"
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
           Module *parent_) :
    Graphics_Object(name_, RECT, parent_, location_, color_)
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
    if(!SELECTING_SRC
       || (SELECTING_SRC && parent != NULL && parent->mouse_over())
       || name == "background rect")
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                               color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                               color.a / 2);
    }

    if(fill)
    {
        SDL_RenderFillRect(RENDERER, &location);
    }
    else
    {
        SDL_RenderDrawRect(RENDERER, &location);
    }
}

/*
 * Send this object to the handle event function of its listener.
 */
void Rect::clicked()
{
    if(listener != nullptr)
    {
        std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT
                  << std::endl;

        if(listener->handle_event(this))
        {
            OBJECT_CLICKED = true;
        }
    }
}

