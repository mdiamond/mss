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
Input_Toggle_Button::Input_Toggle_Button(std::string name_, SDL_Rect location_,
                                         SDL_Color *color_,
                                         SDL_Color *color_off_,
                                         SDL_Color *text_color_on_,
                                         SDL_Color *text_color_off_,
                                         TTF_Font *font_,
                                         std::string text_on_,
                                         std::string text_off_, bool b_,
                                         Module *parent, int input_num_,
                                         Input_Text_Box *input_text_box_) :
    Toggle_Button(name_, location_, color_, color_off_, text_color_on_,
                 text_color_off_, font_, text_on_, text_off_, b_, parent_),
    input_num(input_num_),
    input_text_box(input_text_box_)
{
    // Overwrite the default type of a toggle button
    graphics_object_type = INPUT_TOGGLE_BUTTON;
}

/*
 * Destructor.
 */
Input_Toggle_Button::~Input_Toggle_Button()
{}

/*
 * If the input is live, cancel input, otherwise, enter input select mode and
 * set this as the current input toggle button. If canceling input, ensure
 * that this input toggle button's associated input text box displays either
 * the correct value for the parameter, or "input" if the parameter must be
 * live for processing.
 */
void Input_Toggle_Button::toggle()
{
    if(parent->inputs[input_num].live)
    {
        parent->cancel_input(input_num);
        input_text_box->update_current_text(input_text_box->prompt_text.text);
        if(!(input_text_box->prompt_text.text == "input"))
        {
            float input val = parent->inputs[input_num].val;
            std::string input_val_str = std::to_string(input_val);
            input_text_box->update_current_text(input_val_str);
        }
        b = false;
    }

    else
    {
        SELECTING_SRC = !SELECTING_SRC;
        CURRENT_INPUT_TOGGLE_BUTTON = this;
    }
}

