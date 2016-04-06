/*
 * Matthew Diamond 2015
 * The Input Text Box graphics object. This object allows the user
 * to type into it to select inputs or change parameters.
 */

#ifndef synth_input_text_box_h
#define synth_input_text_box_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

// Included classes
#include "Rect.hpp"
#include "Text.hpp"
#include "Text_Box.hpp"

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
};

#endif
