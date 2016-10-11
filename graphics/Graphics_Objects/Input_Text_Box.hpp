/*
 * Matthew Diamond 2015
 * The Input Text Box graphics object. This object allows the user
 * to type into it to select inputs or change parameters.
 */

#ifndef MSS_INPUT_TEXT_BOX_HPP
#define MSS_INPUT_TEXT_BOX_HPP

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL.h"

// Included graphics classes
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Text_Box.hpp"

// Forward declare input text box
class Input_Toggle_Button;

/***********************************
 * INPUT TEXT BOX CLASS DEFINITION *
 ***********************************/

class Input_Text_Box: public Text_Box
{
public:
    int input_num;
    Input_Toggle_Button *input_toggle_button;

    // Constructor and destructor
    Input_Text_Box(std::string, SDL_Rect, SDL_Color *, SDL_Color *,
                   std::string, TTF_Font *, Module *, int,
                   Input_Toggle_Button *);
    virtual ~Input_Text_Box();

    // Member functions
    //   Override the default text box entered() function
    void entered();
    //   Check if a string can be turned into a float
    bool can_floatify(std::string *);
};

#endif

