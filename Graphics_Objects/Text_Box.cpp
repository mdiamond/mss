/*
 * Matthew Diamond 2015
 * Member functions for the Text Box class.
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
#include "Text_Box.hpp"

using namespace std;

/*****************************
 * TEXT BOX MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Text_Box::Text_Box(string *_name, SDL_Rect *_location, SDL_Color *_color, string *_text, TTF_Font *_font)
{
    name = *_name;
    type = TEXT;
    location = *_location;
    color = *_color;

    font = _font;
    text = *_text;
    old_text = text;

    SDL_Surface *surface = TTF_RenderText_Blended(font, (text).c_str(), color);
    texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    location.w = width;
    location.h = height;
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}

/*
 * Dummy function.
 */
Text_Box::~Text_Box()
{

}

/*
 * Render the text box. If the text has changed, first
 * re-create the texture. If the text box is active, draw a flashing
 * cursor within it.
 */
void Text_Box::render_graphics_object()
{
    if(text != old_text)
    {
        SDL_Surface *surface = TTF_RenderText_Blended(font, (text).c_str(), color);
        texture = SDL_CreateTextureFromSurface(RENDERER, surface);
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        location.w = width;
        location.h = height;
        SDL_RenderCopy(RENDERER, texture, NULL, &location);
        old_text = text;
    }
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
    if(active && CURSOR_ON)
    {
        SDL_SetRenderDrawColor(RENDERER, 256 - color.r, 256 - color.g, 256 - color.b, color.a);
        SDL_RenderDrawLine(RENDERER, location.x + location.w, location.y + 2,
                           location.x + location.w, location.y + location.h - 2);
    }
}

void Text_Box::clicked()
{
    if(!active)
    {
        active = true;
        ACTIVE_TEXT_BOX = this;
    }
}
