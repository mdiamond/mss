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
#include "SDL2/SDL.h"

// Included files
#include "../function_forwarder.hpp"
#include "../main.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Rect.hpp"

/*************************
 * RECT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Rect::Rect(std::string _name, SDL_Rect _location, SDL_Color *_color, Module *_parent) :
    Graphics_Object(_name, RECT, _parent, _location, _color)
{
    fill = true;
}

/*
 * Destructor.
 */
Rect::~Rect()
{

}

/*
 * Render the rectangle. If fill is set, fill it, otherwise,
 * just draw the outline.
 */
void Rect::render()
{
    if(!SELECTING_SRC || name == "background (rect)" 
       || (SELECTING_SRC && parent != NULL
       && parent->graphics_objects[0]->was_clicked()))
        SDL_SetRenderDrawColor(RENDERER, color->r, color->g, color->b, color->a);
    else
        SDL_SetRenderDrawColor(RENDERER, color->r, color->g, color->b, color->a / 2);
    if(fill)
        SDL_RenderFillRect(RENDERER, &location);
    else
        SDL_RenderDrawRect(RENDERER, &location);
}

/*
 * Rectangles only respond to clicks during input select mode.
 * When a rectangle is clicked it will send itself to the function forwarder,
 * which will use the parent of the rectangle as the source module for input.
 */
void Rect::clicked()
{
    if(parent != NULL)
    {
        std::cout << PINK_STDOUT << parent->name << " clicked" << DEFAULT_STDOUT << std::endl;

        function_forwarder(this);
        OBJECT_CLICKED = true;
    }
}

/*
 * Set this text object's color.
 */
void Rect::set_color(SDL_Color *_color)
{
    color = _color;
    updated = true;
}
