/*
 * Matthew Diamond 2015
 * Member functions for the Text class.
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
#include "../main.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Text.hpp"

using namespace std;

/*************************
 * TEXT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Text::Text(string *_name, SDL_Rect *_location, SDL_Color *_color,
           string *_live_text, string *_original_text, TTF_Font *_font)
{
    name = *_name;
    type = TEXT;
    location = *_location;
    color = *_color;

    font = _font;
    original_text = *_original_text;
    current_text = original_text;
    old_text = *_original_text;
    live_text = _live_text;

    SDL_Surface *surface = TTF_RenderText_Blended(font, (current_text).c_str(), color);
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
Text::~Text()
{

}

/*
 * Render the text. If the text has changed, first
 * re-create the texture.
 */
void Text::render_graphics_object()
{
    if(live_text != NULL && current_text != old_text)
    {
        old_text = current_text;
        current_text = *live_text;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(font, (current_text).c_str(), color);
    texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    location.w = width;
    location.h = height;
    SDL_RenderCopy(RENDERER, texture, NULL, &location);

    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}

/*
 * Dummy function.
 */
void Text::clicked()
{

}
