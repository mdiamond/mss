/*
 * Matthew Diamond 2015
 * Member functions for the Input Toggle Button class.
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
#include "Input_Toggle_Button.hpp"

using namespace std;

/****************************************
 * INPUT TOGGLE BUTTON MEMBER FUNCTIONS *
 ****************************************/

/*
 * Constructor.
 */
Input_Toggle_Button::Input_Toggle_Button(string _name, SDL_Rect _location, SDL_Color _color,
                             SDL_Color _color_off, SDL_Color _text_color_on,
                             SDL_Color _text_color_off, TTF_Font *_font, string _text_on,
                             string _text_off, bool _b, Module *_parent, int _input_num) :
    Toggle_Button(_name, _location, _color, _color_off, _text_color_on, _text_color_off,
                  _font, _text_on, _text_off, _b, _parent),
    input_num(_input_num)
{
    // Overwrite the default type of a toggle button
    type = INPUT_TOGGLE_BUTTON;
}

/*
 * Destructor.
 */
Input_Toggle_Button::~Input_Toggle_Button()
{

}

/*
 * Render the Toggle_Button.
 */
void Input_Toggle_Button::render()
{
    if(parent->inputs_live[input_num])
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(RENDERER, &location);
        SDL_SetRenderDrawColor(RENDERER, color_off.r, color_off.g, color_off.b, color_off.a);
        SDL_RenderDrawRect(RENDERER, &location);
        text_on.location.x = location.x + (location.w - text_on.location.w) / 2;
        text_on.render();
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color_off.r, color_off.g, color_off.b, color_off.a);
        SDL_RenderFillRect(RENDERER, &location);
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(RENDERER, &location);
        text_off.location.x = location.x + (location.w - text_off.location.w) / 2;
        text_off.render();
    }
}

void Input_Toggle_Button::clicked()
{
    if(!OBJECT_CLICKED)
    {
        cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT << endl;

        toggle();
        function_forwarder(this);
        OBJECT_CLICKED = true;
    }
}

void Input_Toggle_Button::toggle()
{
    if(parent->inputs_live[input_num])
        parent->cancel_input(input_num);

    else
    {
        SELECTING_SRC = !SELECTING_SRC;
        CURRENT_INPUT_TOGGLE_BUTTON = this;
    }
}
