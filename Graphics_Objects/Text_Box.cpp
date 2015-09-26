/*
 * Matthew Diamond 2015
 * Member functions for the Text Box class.
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
#include "../utils.hpp"

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
Text_Box::Text_Box(string *_name, SDL_Rect *_location, SDL_Color *_color,
                   string *_live_text, string *_original_text,
                   string *_prompt_text, TTF_Font *_font, Module *_parent)
{
    name = *_name;
    type = TEXT_BOX;
    location = *_location;
    color = *_color;
    parent = _parent;

    font = _font;
    string idle_text_name = "idle text (text)";
    string prompt_text_name = "prompt text (text)";
    string typing_text_name = "typing text (text)";
    text = new Text(&idle_text_name, _location, &BLACK, _live_text,
                         _original_text, _font);
    prompt_text = new Text(&prompt_text_name, _location, &BLACK,
                           NULL, _prompt_text, _font);
    typing_text = new Text(&typing_text_name, _location, &BLACK,
                           &TYPING_BUFFER, &TYPING_BUFFER, _font);
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
    SDL_SetRenderDrawColor(RENDERER,
                           WHITE.r,
                           WHITE.g,
                           WHITE.b,
                           WHITE.a);
    SDL_RenderFillRect(RENDERER, &location);

    if(active)
    {
        typing_text->render_graphics_object();
    }
    else
    {
        if(text->current_text == "")
            prompt_text->render_graphics_object();
        else
            text->render_graphics_object();
    }

    if(active && CURSOR_ON)
    {
        SDL_SetRenderDrawColor(RENDERER, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderDrawLine(RENDERER, typing_text->location.x + typing_text->location.w,
                           typing_text->location.y + 2,
                           typing_text->location.x + typing_text->location.w,
                           typing_text->location.y + typing_text->location.h - 2);
    }

}

void Text_Box::clicked()
{
    if(!active)
    {
        active = true;
        ACTIVE_TEXT_BOX = this;
        TYPING_BUFFER = "";
        SDL_SetTextInputRect(&location);
        SDL_StartTextInput();
    }
}

void Text_Box::entered()
{
    function_forwarder(this);
    SDL_StopTextInput();
    ACTIVE_TEXT_BOX = NULL;
    active = false;
}
