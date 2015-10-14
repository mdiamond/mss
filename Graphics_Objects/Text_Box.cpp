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
#include "../function_forwarder.hpp"

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
    text = new Text(&idle_text_name, _location, &BLACK, _original_text, _font);
    prompt_text = new Text(&prompt_text_name, _location, &BLACK, _prompt_text, _font);
    typing_text = new Text(&typing_text_name, _location, &BLACK, _original_text, _font);

    active = false;
}

/*
 * Destructor.
 */
Text_Box::~Text_Box()
{
    delete text;
    delete prompt_text;
    delete typing_text;
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
        typing_text->render_graphics_object();
    else
    {
        if(text->text == "")
            prompt_text->render_graphics_object();
        else
            text->render_graphics_object();
    }

    if(active && CURSOR_ON)
    {
        SDL_SetRenderDrawColor(RENDERER, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderDrawLine(RENDERER, typing_text->location.x + typing_text->location.w,
                           location.y + 2,
                           location.x + typing_text->location.w,
                           location.y + 11);
    }
}

void Text_Box::typed(char *ch)
{
    typing_text->text += ch;
    typing_text->updated = true;
}

void Text_Box::delete_character()
{
    if(!typing_text->text.empty())
        typing_text->text.pop_back();

    typing_text->updated = true;
}

void Text_Box::clicked()
{
    cout << BLUE_STDOUT << name << " clicked" << DEFAULT_STDOUT << endl;

    if(!active)
    {
        active = true;
        ACTIVE_TEXT_BOX = this;
        SDL_SetTextInputRect(&location);
        SDL_StartTextInput();
    }
}

void Text_Box::entered()
{
    SDL_StopTextInput();
    function_forwarder(this);
    text->text = typing_text->text;
    text->updated = true;
    ACTIVE_TEXT_BOX = NULL;
    active = false;
    typing_text->text = "";
}
