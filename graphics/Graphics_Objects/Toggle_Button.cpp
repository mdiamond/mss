/*
 * Matthew Diamond 2015
 * Member functions for the Toggle Button class.
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
                             SDL_Color color_, SDL_Color color_off_,
                             SDL_Color text_color_on_,
                             SDL_Color text_color_off_,
                             std::string text_on_, std::string text_off_,
                             bool b_, Graphics_Listener *listener_) :
    Button(name_, location_, color_, text_color_on_, text_on_, listener_),
    b(b_), color_off(color_off_),
    text_off(Text(name_ + " text (off)", location_, text_color_off_,
                  text_off_)),
    background_off(Rect(name_ + " background rect (off)", location_,
                        color_off_, NULL))
{
    graphics_object_type = TOGGLE_BUTTON;
}

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
        Button::render();
    }
    else
    {
        background_off.render();
        text_off.render();
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
    Button::update_location(location);
    background_off.update_location(location_);
    text_off.update_location(location_);
    updated = true;
}

/*
 * Updated this toggle button's colors.
 */
void Toggle_Button::set_colors(SDL_Color color_, SDL_Color color_off_,
                               SDL_Color text_color_,
                               SDL_Color text_color_off_)
{
    Button::set_colors(color_, text_color_);
    background_off.set_color(color_off_);
    text_off.set_color(text_color_off_);
    updated = true;
}

