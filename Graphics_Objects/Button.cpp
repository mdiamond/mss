/*
 * Matthew Diamond 2015
 * Member functions for the Button class.
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
#include "Text.hpp"
#include "Button.hpp"

using namespace std;

/***************************
 * BUTTON MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Button::Button(string _name, SDL_Rect _location, SDL_Color _color,
               SDL_Color _text_color, string _text, Module *_parent) :
    Graphics_Object(_name, BUTTON, _parent, _location, _color),
    text_str(_text),
    text(Text("button text (text)", _location, _text_color, text_str, FONT_REGULAR))
{
    // Make the text start 1 pixel away from the edge of the containing box
    text.location.x += 1;
}

/*
 * Destructor.
 */
Button::~Button()
{

}

/*
 * Render the Button.
 */
void Button::render()
{
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(RENDERER, &location);
    text.render();
}

/*
 * Send this graphics object to the function forwarder,
 * which will determine what its intended functionality is
 * and call the appropriate functions.
 */
void Button::clicked()
{
    cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT << endl;

    if(!OBJECT_CLICKED)
    {
        function_forwarder(this);
        OBJECT_CLICKED = true;
    }
}
