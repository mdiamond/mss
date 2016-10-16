/*
 * Matthew Diamond 2015
 * The Button graphics object. This object is just a text box with the ability
 * to be clicked in order to call some function.
 */

#ifndef MSS_BUTTON_HPP
#define MSS_BUTTON_HPP

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL.h"

// Included graphics classes
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"

/***************************
 * BUTTON CLASS DEFINITION *
 ***************************/

class Button: public Graphics_Object
{
public:
    Rect background;
    Text text;

    // Constructor and destructor
    Button(std::string, SDL_Rect, SDL_Color, SDL_Color, std::string,
           Module *);
    virtual ~Button();

    // Member functions
    virtual void render();
    virtual void clicked();
    //   Override the default graphics object update_location() function
    void update_location(SDL_Rect);
    //   Set the colors of this button
    void set_colors(SDL_Color, SDL_Color);
};

#endif

