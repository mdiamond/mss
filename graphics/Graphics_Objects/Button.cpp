/*
 * Matthew Diamond 2015
 * Member functions for the Button class.
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
#include "function_forwarder.hpp"

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
               SDL_Color text_color_, std::string text_, Module *parent_) :
    Graphics_Object(name_, BUTTON, parent_, location_, color_),
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

/*
 * If this button has no listener, send it to the function forwarder.
 * Otherwise, send it to the event handling function of its listener.
 */
void Button::clicked()
{
    std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT
              << std::endl;

    if(!OBJECT_CLICKED)
    {
        if(listener == nullptr)
        {
            function_forwarder(this);
        }
        else
        {
            listener->handle_event(this);
        }
        OBJECT_CLICKED = true;
    }
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

