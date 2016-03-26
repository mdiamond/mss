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
#include "Rect.hpp"
#include "Text.hpp"

/***************************
 * BUTTON CLASS DEFINITION *
 ***************************/

class Button: public Graphics_Object
{
    public:
        std::string text_str;
        Rect background;
        Text text;
        void (Module::*button_function) (Button *);

        // Constructor and destructor
        Button(std::string, SDL_Rect, SDL_Color *,
               SDL_Color *, std::string, Module *);
        virtual ~Button();

        // Virtual member functions
        virtual void render();
        virtual void clicked();

        // Member functions
        //   Override the default graphics object update_location() function
        void update_location(SDL_Rect);
};

#endif
