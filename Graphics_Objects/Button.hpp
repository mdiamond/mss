/*
 * Matthew Diamond 2015
 * The Button graphics object. This object is
 * just a text box with the ability to be clicked in order
 * to call some function.
 */

#ifndef synth_button_h
#define synth_button_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

// Included classes
#include "Text.hpp"

/***************************
 * BUTTON CLASS DEFINITION *
 ***************************/

class Button: public Graphics_Object
{
    public:
        std::string text_str;
        Text *text;
        // Constructor and destructor
        Button(std::string *, SDL_Rect *, SDL_Color *, std::string *,
               Module *);
        virtual ~Button();
        // Member functions
        virtual void render_graphics_object();
        virtual void clicked();
};

#endif
