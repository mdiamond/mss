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

using namespace std;

/*************************
 * RECT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Rect::Rect(string _name, SDL_Rect *_location, SDL_Color *_color)
{
    name = _name;
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
 * Render the rectangle. If fill is set, fill it, otherwise,
 * just draw the outline.
 */
void Rect::render()
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
    if(!OBJECT_CLICKED && SELECTING_SRC)
    {
        cout << BLUE_STDOUT << parent->name << " clicked" << DEFAULT_STDOUT << endl;

        function_forwarder(this);
        OBJECT_CLICKED = true;
    }
}
