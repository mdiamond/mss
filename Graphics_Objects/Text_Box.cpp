/*
 * Matthew Diamond 2015
 * Member functions for the Text Box class.
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
#include "Text_Box.hpp"

using namespace std;

/*****************************
 * TEXT BOX MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Text_Box::Text_Box(string *_name, SDL_Rect *_location, SDL_Color *_color, string *_live_text, string *_original_text, TTF_Font *_font)
{
    name = *_name;
    type = TEXT;
    location = *_location;
    color = *_color;

    font = _font;
    string idle_text_name = "idle text";
    string typing_text_name = "typing text";
    idle_text = new Text(&idle_text_name, _location, _color, _live_text, _original_text, _font);
    typing_text = new Text(&typing_text_name, _location, _color, &typing_buffer, _original_text, _font);
    typing_buffer = "";
}

/*
 * Dummy function.
 */
Text_Box::~Text_Box()
{

}

/*
 * Render the text box. If the text has changed, first
 * update the text. If the text box is active, draw a flashing
 * cursor within it.
 */
void Text_Box::render_graphics_object()
{
    if(active)
        typing_text->render_graphics_object();
    else
        idle_text->render_graphics_object();

    if(active && CURSOR_ON)
    {
        SDL_SetRenderDrawColor(RENDERER, 256 - color.r, 256 - color.g, 256 - color.b, color.a);
        SDL_RenderDrawLine(RENDERER, location.x + location.w, location.y + 2,
                           location.x + location.w, location.y + location.h - 2);
    }
}

void Text_Box::clicked()
{
    if(!active)
    {
        active = true;
        ACTIVE_TEXT_BOX = this;
    }
}
