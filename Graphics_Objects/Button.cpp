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
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"
#include "../function_forwarder.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Text.hpp"
#include "Button.hpp"

/***************************
 * BUTTON MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Button::Button(std::string _name, SDL_Rect _location, SDL_Color *_color,
               SDL_Color *_text_color, std::string _text, Module *_parent) :
    Graphics_Object(_name, BUTTON, _parent, _location, _color),
    text_str(_text),
    background(Rect(name + " background rect (rect)", location, color, NULL)),
    text(Text("button text (text)", _location, _text_color, text_str, FONT_REGULAR)),
    button_function(&Module::button_function)
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
 * Send this graphics object to the function forwarder,
 * which will determine what its intended functionality is
 * and call the appropriate functions.
 */
void Button::clicked()
{
    std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT << std::endl;

    if(!OBJECT_CLICKED)
    {
        function_forwarder(this);
        (parent->*button_function)(this);
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
