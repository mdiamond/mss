/*
 * Matthew Diamond 2015
 * The Input Toggle Button graphics object. This object is
 * just a text box with the ability to be clicked in order
 * to toggle a boolean associated with the button. In addition,
 * and unlike a normal toggle button, it has an associated text box
 * into which it will automatically enter module names for input
 * when a module is selected using this type of toggle button.
 */

#ifndef synth_input_toggle_button_h
#define synth_input_toggle_button_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

// Included classes
#include "Input_Text_Box.hpp"
#include "Text.hpp"
#include "Toggle_Button.hpp"

/****************************************
 * INPUT TOGGLE BUTTON CLASS DEFINITION *
 ****************************************/

class Input_Toggle_Button: public Toggle_Button
{
    public:
        int input_num;
        Input_Text_Box *input_text_box;
        // Constructor and destructor
        Input_Toggle_Button(std::string, SDL_Rect, SDL_Color,
                            SDL_Color, SDL_Color, SDL_Color,
                            TTF_Font *,
                            std::string, std::string,
                            bool, Module *, int, Input_Text_Box *);
        virtual ~Input_Toggle_Button();
        void toggle();
};

#endif