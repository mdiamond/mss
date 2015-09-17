/*
 * Matthew Diamond 2015
 * Member functions for the Text class.
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
#include "Text.hpp"

using namespace std;

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Text::Text(string *_name, SDL_Rect *_location, SDL_Color *_color, SDL_Texture *_texture)
{
    name = *_name;
    location = *_location;
    color = *_color;
    texture = _texture;
    type = TEXT;
}

/*
 * Dummy function.
 */
Text::~Text()
{

}

/*
 * Render the rectangle. If fill is set, fill it, otherwise,
 * just draw the outline.
 */
void Text::render_graphics_object()
{
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}