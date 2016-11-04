/*
 * Matthew Diamond 2015
 * Member functions for the Text Box class.
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
#include "module_utils.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"

/***********************************
 * INPUT TEXT BOX MEMBER FUNCTIONS *
 ***********************************/

/*
 * Constructor.
 */
Input_Text_Box::Input_Text_Box(std::string name_, SDL_Rect location_,
                               SDL_Color color_, SDL_Color text_color_,
                               std::string prompt_text_, Module *parent_,
                               int input_num_,
                               Input_Toggle_Button *input_toggle_button_) :
    Text_Box(name_, location_, color_, text_color_, prompt_text_, FONT,
             parent_),
    input_num(input_num_),
    input_toggle_button(input_toggle_button_)
{
    // Override the default type of a text box
    graphics_object_type = INPUT_TEXT_BOX;
}

/*
 * Destructor.
 */
Input_Text_Box::~Input_Text_Box()
{}

/*
 * Return true if it is possible to turn this string into a float. Return false
 * otherwise.
 */
bool Input_Text_Box::can_floatify(std::string *string)
{
    if(string->empty())
    {
        return false;
    }
    if(!(isdigit((*string)[0]) || (*string)[0] == '-'
         || (*string)[0] == '.'))
    {
        return false;
    }
    if(string->size() > 1)
        if(!(isdigit((*string)[1])
             || (*string)[1] == '.'))
        {
            return false;
        }
    return true;
}

/*
 * Stop SDL text input, send this graphics object to the function forwarder,
 * set the current text to be what is in the typing buffer, then set this text
 * box as updated. After that, deactivate this text box and clear the typing
 * buffer.
 */
void Input_Text_Box::entered()
{
    Module *src = NULL;
    float val = 0;

    std::cout << PINK_STDOUT << name << " entered" << DEFAULT_STDOUT
              << std::endl;

    SDL_StopTextInput();

    if(typing_text.text != "")
    {
        if(can_floatify(&typing_text.text))
        {
            val = std::stof(typing_text.text.c_str());
            parent->set(val, input_num);
        }
        else
        {
            src = find_module_as_source(&typing_text.text, &MODULES, parent);
            if(src != NULL)
            {
                typing_text.text = typing_text.text.substr(0, 3) + " "
                                   + typing_text.text.substr(typing_text.text.find(" ") + 1);
                parent->set(src, input_num);
                input_toggle_button->b = true;
                text.text = typing_text.text;
                text.updated = true;
            }
        }
    }
    else
    {
        std::cout << RED_STDOUT
                  << "entered string contained no characters, idiot"
                  << DEFAULT_STDOUT << std::endl;
    }

    ACTIVE_TEXT_BOX = NULL;
    active = false;
    typing_text.text = "";
    typing_text.updated = true;
}

