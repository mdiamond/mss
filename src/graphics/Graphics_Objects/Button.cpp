/*
 * Matthew Diamond 2015
 * Member functions for the Button class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Button.hpp"

/***************************
 * BUTTON MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Button::Button(std::string name_, SDL_Rect location_, SDL_Color color_,
               SDL_Color text_color_, std::string text_,
               Graphics_Listener *listener_) :
    Graphics_Object(name_, BUTTON, listener_, location_, color_),
    background(Rect(name_ + " background rect", location_, color_,
                    NULL)),
    text(Text(name_ + " text", location_, text_color_, text_))
{
    // Make the text start 1 pixel away from the edge of the containing box
    text.location.x += 1;
}

/*
 * Destructor.
 */
Button::~Button()
{}

/*
 * Render the Button.
 */
void Button::render()
{
    background.render();
    text.render();
}

void Button::update_location(SDL_Rect location_)
{
    location = location_;
    background.update_location(location_);
    text.update_location(location_);
    updated = true;
}

void Button::set_colors(SDL_Color color_, SDL_Color text_color_)
{
    background.set_color(color_);
    text.set_color(text_color_);
    updated = true;
}

