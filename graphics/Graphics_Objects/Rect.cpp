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
    if(!SELECTING_SRC || (SELECTING_SRC && parent != NULL && parent->was_clicked()) ||
       name == "background (rect)")
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
 * When a rectangle is clicked it will call upon its parent module to become
 * a source for the parent of the active input toggle box
 */
void Rect::clicked()
{
    if(parent != NULL)
    {
        std::cout << PINK_STDOUT << parent->name << " clicked" << DEFAULT_STDOUT << std::endl;

        // If it is currently module selection mode (there is an active
        // input toggle button and we are selecting a source), then this
        // module has been selected as a source for some input
        if(SELECTING_SRC && CURRENT_INPUT_TOGGLE_BUTTON != NULL &&
           CURRENT_INPUT_TOGGLE_BUTTON->parent != parent)
        {
            parent->module_selected();
            OBJECT_CLICKED = true;
        }
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

