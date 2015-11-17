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
                             string _text_off, bool _b, Module *_parent, int _input_num,
                             Input_Text_Box *_input_text_box) :
    Toggle_Button(_name, _location, _color, _color_off, _text_color_on, _text_color_off,
                  _font, _text_on, _text_off, _b, _parent),
    input_num(_input_num),
    input_text_box(_input_text_box)
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

void Input_Toggle_Button::toggle()
{
    if(parent->inputs_live[input_num])
    {
        parent->cancel_input(input_num);
        input_text_box->text.text = input_text_box->prompt_text.text;
    }

    else
    {
        SELECTING_SRC = !SELECTING_SRC;
        CURRENT_INPUT_TOGGLE_BUTTON = this;
    }
}
