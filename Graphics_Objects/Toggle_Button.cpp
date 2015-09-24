/*
 * Matthew Diamond 2015
 * Member functions for the Toggle Button class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"

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
Toggle_Button::Toggle_Button(string *_name, SDL_Rect *_location, SDL_Color *_color, SDL_Color *_color_off, string *_text_on, string *_text_off, bool *_b)
{
    name = *_name;
    type = TOGGLE_BUTTON;
    location = *_location;
    color = *_color;
    color_off = *_color_off;
    string text_on_name = "text on";
    string text_off_name = "text off";
    text_on_str = *_text_on;
    text_off_str = *_text_off;
    text_on = new Text(&text_on_name, &location, &color_off, &text_on_str, FONT_REGULAR);
    text_off = new Text(&text_off_name, &location, &color, &text_off_str, FONT_REGULAR);
    b = _b;
}

/*
 * Dummy function.
 */
Toggle_Button::~Toggle_Button()
{

}

/*
 * Render the Toggle_Button.
 */
void Toggle_Button::render_graphics_object()
{
    if(*b)
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(RENDERER, &location);
        text_on->render_graphics_object();
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color_off.r, color_off.g, color_off.b, color_off.a);
        SDL_RenderFillRect(RENDERER, &location);
        text_off->render_graphics_object();
    }
}

void Toggle_Button::clicked()
{
    *b = !(*b);
}
