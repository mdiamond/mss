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
        // The boolean variable, the background color when off,
        // the text when on, the text when off, the background when on,
        // and the background when off
        bool b;
        SDL_Color *color_off;
        Text text_on, text_off;
        Rect background;
        Rect background_off;
        // Constructor and destructor

        Toggle_Button(std::string, SDL_Rect, SDL_Color *,
                      SDL_Color *, SDL_Color *, SDL_Color *,
                      TTF_Font *,
                      std::string, std::string,
                      bool, Module *);
        virtual ~Toggle_Button();

        // Member functions
        void render();
        void clicked();
        //   Toggle this button
        virtual void toggle();
        //   Override the default graphics object update_location() function
        void update_location(SDL_Rect);
        //   Set this toggle button's colors
        void set_colors(SDL_Color *, SDL_Color *, SDL_Color *, SDL_Color *);
};

#endif
