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

/*************************
 * TEXT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Text::Text(std::string _name, SDL_Rect _location, SDL_Color _color,
           std::string _text, TTF_Font *_font) :
    Graphics_Object(_name, TEXT, NULL, _location, _color),
    font(_font), text(_text)
{
    // Render the text for the first time
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    location.w = width;
    location.h = height;
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
    updated = false;
}

/*
 * Destructor.
 */
Text::~Text()
{
    SDL_DestroyTexture(texture);
}

/*
 * Render the text. If the text has changed, first
 * re-create the texture.
 */
void Text::render()
{
    if(updated)
    {
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
        SDL_DestroyTexture(texture);
        texture = SDL_CreateTextureFromSurface(RENDERER, surface);
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        location.w = width;
        location.h = height;
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderCopy(RENDERER, texture, NULL, &location);
        updated = false;
    }

    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}

/*
 * Text objects do not respond to clicks.
 */
void Text::clicked()
{

}

/*
 * A function for updating the text in this object.
 */
void Text::update_text(std::string s)
{
    text = s;
    updated = true;
}
