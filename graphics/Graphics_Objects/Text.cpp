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
Text::Text(std::string name_, SDL_Rect location_, SDL_Color color_,
           std::string text_) :
    Graphics_Object(name_, TEXT, NULL, location_, color_),
    font(FONT), text(text_)
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
        texture = SDL_CreateTextureFromSurface(RENDERER, surface);
        SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
        if(text == "")
        {
            location.w = 0;
            location.h = 0;
        }
        if(location.w >= max_width)
        {
            location.w = max_width;
        }
        updated = false;
    }

    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b,
                           color.a);
    SDL_RenderCopy(RENDERER, texture, NULL, &location);
}

/*
 * Text objects do not respond to clicks.
 */
bool Text::clicked()
{
    return false;
}

/*
 * Update this text object's stored text.
 */
void Text::update_text(std::string text_)
{
    text = text_;
    updated = true;
}

