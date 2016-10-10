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
#include "SDL.h"

// Included files
#include "main.hpp"
#include "function_forwarder.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"

/****************************************
 * INPUT TOGGLE BUTTON MEMBER FUNCTIONS *
 ****************************************/

/*
 * Constructor.
 */
Input_Toggle_Button::Input_Toggle_Button(std::string _name, SDL_Rect _location, SDL_Color *_color,
                             SDL_Color *_color_off, SDL_Color *_text_color_on,
                             SDL_Color *_text_color_off, TTF_Font *_font, std::string _text_on,
                             std::string _text_off, bool _b, Module *_parent, int _input_num,
                             Input_Text_Box *_input_text_box) :
    Toggle_Button(_name, _location, _color, _color_off, _text_color_on, _text_color_off,
                  _font, _text_on, _text_off, _b, _parent),
    input_num(_input_num),
    input_text_box(_input_text_box)
{
    // Overwrite the default type of a toggle button
    graphics_object_type = INPUT_TOGGLE_BUTTON;
}

/*
 * Destructor.
 */
Input_Toggle_Button::~Input_Toggle_Button()
{

}

/*
 * If the input is live, cancel input,
 * otherwise, enter input select mode and
 * set this as the current input toggle button.
 */
void Input_Toggle_Button::toggle()
{
    if(parent->inputs[input_num].live)
    {
        parent->cancel_input(input_num);
        input_text_box->update_current_text(input_text_box->prompt_text.text);
        if(!(input_text_box->prompt_text.text == "input"))
            input_text_box->update_current_text(std::to_string(parent->inputs[input_num].val));
        b = false;
    }

    else
    {
        SELECTING_SRC = !SELECTING_SRC;
        CURRENT_INPUT_TOGGLE_BUTTON = this;
    }
}
