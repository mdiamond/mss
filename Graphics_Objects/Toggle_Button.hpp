/*
 * Matthew Diamond 2015
 * The Toggle Button graphics object. This object is
 * just a text box with the ability to be clicked in order
 * to toggle a boolean associated with the button.
 */

#ifndef synth_toggle_button_h
#define synth_toggle_button_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

// Included classes
#include "Rect.hpp"
#include "Text.hpp"

/**********************************
 * TOGGLE BUTTON CLASS DEFINITION *
 **********************************/

class Toggle_Button: public Graphics_Object
{
    public:
        bool b;
        SDL_Color color_off;
        Text text_on, text_off;
        Rect background;
        Rect background_off;
        // Constructor and destructor
        Toggle_Button(std::string, SDL_Rect, SDL_Color,
                      SDL_Color, SDL_Color, SDL_Color,
                      TTF_Font *,
                      std::string, std::string,
                      bool, Module *);
        virtual ~Toggle_Button();
        // Member functions
        virtual void render();
        virtual void clicked();
        void toggle();
};

#endif
