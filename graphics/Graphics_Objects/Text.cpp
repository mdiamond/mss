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
#include "SDL.h"

// Included files
#include "main.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Text.hpp"

/*************************
 * TEXT MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Text::Text(std::string _name, SDL_Rect _location, SDL_Color _color,
           std::string _text) :
    Graphics_Object(_name, TEXT, NULL, _location, _color),
    font(FONT), text(_text)
{
    // Render the text for the first time
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
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
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(),
                                                      color);
        SDL_DestroyTexture(texture);
        texture = SDL_CreateTextureFromSurface(RENDERER, surface);
        SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
        updated = false;
    }

    if(!SELECTING_SRC
       || (SELECTING_SRC && parent != NULL && parent->was_clicked()))
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                               color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                               color.a / 2);
    }

    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}

/*
 * Text objects do not respond to clicks.
 */
void Text::clicked()
{

}

/*
 * Update this text object's stored text.
 */
void Text::update_text(std::string text_)
{
    text = text_;
    updated = true;
}

