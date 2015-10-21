/*
 * Matthew Diamond 2015
 * Member functions for the Toggle Button class.
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
#include "Toggle_Button.hpp"

using namespace std;

/**********************************
 * TOGGLE BUTTON MEMBER FUNCTIONS *
 **********************************/

/*
 * Constructor.
 */
Toggle_Button::Toggle_Button(string _name, SDL_Rect *_location, SDL_Color *_color,
                             SDL_Color *_color_off, SDL_Color *_text_color_on,
                             SDL_Color *_text_color_off, TTF_Font *_font, string _text_on,
                             string _text_off, bool _b, Module *_parent)
{
    name = _name;
    type = TOGGLE_BUTTON;
    location = *_location;
    color = *_color;

    color_off = *_color_off;

    text_on = new Text("text on (text)", &location, _text_color_on, _text_on, _font);
    text_off = new Text("text off (text)", &location, _text_color_off, _text_off, _font);

    b = _b;
    parent = _parent;
}

/*
 * Destructor.
 */
Toggle_Button::~Toggle_Button()
{
    delete text_on;
    delete text_off;
}

/*
 * Render the Toggle_Button.
 */
void Toggle_Button::render()
{
    if(b)
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(RENDERER, &location);
        SDL_SetRenderDrawColor(RENDERER, color_off.r, color_off.g, color_off.b, color_off.a);
        SDL_RenderDrawRect(RENDERER, &location);
        text_on->render();
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color_off.r, color_off.g, color_off.b, color_off.a);
        SDL_RenderFillRect(RENDERER, &location);
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(RENDERER, &location);
        text_off->render();
    }
}

/*
 * Send this graphics object to the function forwarder,
 * which will determine what its intended functionality is
 * and call the appropriate functions
 */
void Toggle_Button::clicked()
{
    if(!OBJECT_CLICKED)
    {
        cout << BLUE_STDOUT << name << " clicked" << DEFAULT_STDOUT << endl;

        function_forwarder(this);
        b = !b;
        OBJECT_CLICKED = true;
    }
}
