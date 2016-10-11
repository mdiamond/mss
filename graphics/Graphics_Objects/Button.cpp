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
Button::Button(std::string _name, SDL_Rect _location, SDL_Color *_color,
               SDL_Color *_text_color, std::string _text, Module *_parent) :
    Graphics_Object(_name, BUTTON, _parent, _location, _color),
    background(Rect(name + " background rect (rect)", _location, _color, NULL)),
    text(Text("button text (text)", _location, _text_color, _text, FONT_REGULAR))
{
    // Make the text start 1 pixel away from the edge of the containing box
    text.location.x += 1;
}

/*
 * Destructor.
 */
Button::~Button()
{

}

/*
 * Render the Button.
 */
void Button::render()
{
    background.render();
    text.render();
}

/*
 * If this button has no parent module, send it to the function forwarder.
 * Otherwise, send it to it's parent module's button handling function.
 */
void Button::clicked()
{
    std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT << std::endl;

    if(!OBJECT_CLICKED)
    {
        if(parent == NULL)
            function_forwarder(this);
        else
            parent->button_function(this);
        OBJECT_CLICKED = true;
    }
}

void Button::update_location(SDL_Rect _location)
{
    location = _location;
    background.update_location(_location);
    text.update_location(_location);
    text.updated = true;
}

void Button::set_colors(SDL_Color *_color, SDL_Color *_text_color)
{
    background.color = _color;
    text.color = _text_color;
    background.updated = true;
    text.updated = true;
}

