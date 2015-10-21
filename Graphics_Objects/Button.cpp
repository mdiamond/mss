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
Button::Button(string _name, SDL_Rect *_location, SDL_Color *_color,
               string _text, Module *_parent)
{
    name = _name;
    type = BUTTON;
    location = *_location;
    color = *_color;

    // Text is opposite the color of the button
    SDL_Color opposite;
    opposite.r = 256 - color.r;
    opposite.g = 256 - color.g;
    opposite.b = 256 - color.b;
    opposite.a = color.a;

    text_str = _text;
    text = new Text("button text (text)", &location, &opposite, _text, FONT_REGULAR);

    parent = _parent;
}

/*
 * Destructor.
 */
Button::~Button()
{
    delete text;
}

/*
 * Render the Button.
 */
void Button::render()
{
    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(RENDERER, &location);
    text->render();
}

/*
 * Send this graphics object to the function forwarder,
 * which will determine what its intended functionality is
 * and call the appropriate functions
 */
void Button::clicked()
{
    cout << BLUE_STDOUT << name << " clicked" << DEFAULT_STDOUT << endl;

    if(!OBJECT_CLICKED)
    {
        function_forwarder(this);
        OBJECT_CLICKED = true;
    }
}
