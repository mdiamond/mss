/*
 * Matthew Diamond 2015
 * Member functions for the Toggle Button class.
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
#include "../main.hpp"
#include "../function_forwarder.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Rect.hpp"
#include "Text.hpp"
#include "Toggle_Button.hpp"

/**********************************
 * TOGGLE BUTTON MEMBER FUNCTIONS *
 **********************************/

/*
 * Constructor.
 */
Toggle_Button::Toggle_Button(std::string _name, SDL_Rect _location, SDL_Color *_color,
                             SDL_Color *_color_off, SDL_Color *_text_color_on,
                             SDL_Color *_text_color_off, TTF_Font *_font, std::string _text_on,
                             std::string _text_off, bool _b, Module *_parent) :
    Graphics_Object(_name, TOGGLE_BUTTON, _parent, _location, _color),
    b(_b), color_off(_color_off),
    text_on(Text(name + " text on (text)", location, _text_color_on, _text_on, _font)),
    text_off(Text(name + " text off (text)", location, _text_color_off, _text_off, _font)),
    background(Rect(name + " background rect (rect)", location, color, NULL)),
    background_off(Rect(name + " background off rect (rect)", location, color_off, NULL))
{

}

/*
 * Destructor.
 */
Toggle_Button::~Toggle_Button()
{

}

/*
 * Render the Toggle_Button.
 */
void Toggle_Button::render()
{
    if(b)
    {
        background.render();
        text_on.location.x = location.x + (location.w - text_on.location.w) / 2;
        text_on.render();
    }
    else
    {
        background_off.render();
        text_off.location.x = location.x + (location.w - text_off.location.w) / 2;
        text_off.render();
    }
}

/*
 * If this toggle button is an input toggle button, cast it and call its toggle
 * function. Otherwise, call the normal toggle button toggle function, then send
 * this toggle button to its parent module's toggle button handling function.
 * All toggle buttons are associated with a parent module.
 */
void Toggle_Button::clicked()
{
    if(!OBJECT_CLICKED)
    {
        std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT << std::endl;

        if(graphics_object_type == INPUT_TOGGLE_BUTTON)
            ((Input_Toggle_Button *) this)->toggle();
        else
        {
            toggle();
            parent->toggle_button_function(this);
        }
        OBJECT_CLICKED = true;
    }
}

/*
 * Toggle the stored boolean variable
 */
void Toggle_Button::toggle()
{
    b = !b;
}

void Toggle_Button::update_location(SDL_Rect _location)
{
    location = _location;
    text_on.update_location(_location);
    text_off.update_location(_location);
    background.update_location(_location);
    background_off.update_location(_location);
    text_on.updated = true;
    text_off.updated = true;
}

/*
 * Updated this toggle button's colors.
 */
void Toggle_Button::set_colors(SDL_Color *_color, SDL_Color *_color_off,
                               SDL_Color *_text_color_on, SDL_Color *_text_color_off)
{
    background.set_color(_color);
    background_off.set_color(_color_off);
    text_on.set_color(_text_color_on);
    text_off.set_color(_text_color_off);
    updated = true;
}
