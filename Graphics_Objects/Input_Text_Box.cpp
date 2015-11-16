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
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"
#include "../function_forwarder.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Rect.hpp"
#include "Input_Text_Box.hpp"

using namespace std;

/***********************************
 * INPUT TEXT BOX MEMBER FUNCTIONS *
 ***********************************/

/*
 * Constructor.
 */
Input_Text_Box::Input_Text_Box(string _name, SDL_Rect _location, SDL_Color _color,
                   string _original_text, string _prompt_text, TTF_Font *_font,
                   Module *_parent, int _input_num) :
    Text_Box(_name, _location, _color, _original_text, _prompt_text, _font, _parent),
    input_num(_input_num)
{
    // Override the default type of a text box
    type = INPUT_TEXT_BOX;
}

/*
 * Destructor.
 */
Input_Text_Box::~Input_Text_Box()
{

}

/*
 * Stop SDL text input, send this graphics object to the function
 * forwarder, set the current text to be what is in the typing buffer,
 * then set this text box as updated. After that, deactivate this text box
 * and clear the typing buffer.
 */
void Input_Text_Box::entered()
{
    Module *src = NULL;
    float val = 0;

    cout << PINK_STDOUT << name << " entered" << DEFAULT_STDOUT << endl;

    SDL_StopTextInput();
    text.text = typing_text.text;

    if(can_floatify(&text.text))
    {
        val = stof(text.text.c_str());
        cout << val << endl;
        parent->set(val, input_num);
    }
    else
    {
        src = find_module_as_source(&text.text, &MODULES, parent);
        if(src != NULL)
            text.text = text.text.substr(0, 3) + " " + text.text.substr(text.text.find(" ") + 1);
        parent->set(src, input_num);
    }

    text.updated = true;
    ACTIVE_TEXT_BOX = NULL;
    active = false;
    typing_text.text = "";
}
