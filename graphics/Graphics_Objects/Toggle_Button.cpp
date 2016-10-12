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
#include "main.hpp"
#include "function_forwarder.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"

/**********************************
 * TOGGLE BUTTON MEMBER FUNCTIONS *
 **********************************/

/*
 * Constructor.
 */
Toggle_Button::Toggle_Button(std::string name_, SDL_Rect location_,
                             SDL_Color *color_, SDL_Color *color_off_,
                             SDL_Color *text_color_on_,
                             SDL_Color *text_color_off_, TTF_Font *font_,
                             std::string text_on_, std::string text_off_,
                             bool b_, Module *parent_) :
    Graphics_Object(name_, TOGGLE_BUTTON, parent_, location_, color_),
    b(b_), color_off(color_off_),
    text_on(Text(name_ + " text on (text)", location_, text_color_on_, text_on_,
                 font_)),
    text_off(Text(name_ + " text off (text)", location_, text_color_off_,
                  text_off_, font_)),
    background(Rect(name_ + " background rect (rect)", location_, color_,
                    NULL)),
    background_off(Rect(name_ + " background off rect (rect)", location_,
                        color_off_, NULL))
{}

/*
 * Destructor.
 */
Toggle_Button::~Toggle_Button()
{}

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
        text_off.location.x = location.x + (location.w - text_off.location.w)
                              / 2;
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
        std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT
                  << std::endl;

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

void Toggle_Button::update_location(SDL_Rect location_)
{
    location = location_;
    text_on.update_location(location_);
    text_off.update_location(location_);
    background.update_location(location_);
    background_off.update_location(location_);
    text_on.updated = true;
    text_off.updated = true;
}

/*
 * Updated this toggle button's colors.
 */
void Toggle_Button::set_colors(SDL_Color *color_, SDL_Color *color_off_,
                               SDL_Color *text_color_on_,
                               SDL_Color *text_color_off_)
{
    background.set_color(color_);
    background_off.set_color(color_off_);
    text_on.set_color(text_color_on_);
    text_off.set_color(text_color_off_);
    updated = true;
}

