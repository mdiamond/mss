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
#include "SDL.h"

// Included files
#include "main.hpp"
#include "function_forwarder.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text_Box.hpp"

/*****************************
 * TEXT BOX MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Text_Box::Text_Box(std::string name_, SDL_Rect location_, SDL_Color *color_,
                   SDL_Color *text_color_, std::string prompt_text_,
                   TTF_Font *font_, Module *parent_) :
    Graphics_Object(name_, TEXT_BOX, parent_, location_, color_),
    text_color(text_color_),
    active(false), font(font_),
    background(Rect(name_ + " background rect (rect)", location_, color_,
                    NULL)),
    text(Text(name_ + " idle text (text)", location_, text_color_, "", font_)),
    prompt_text(Text(name_ + " prompt text (text)", location_, text_color_,
                     prompt_text_, font_)),
    typing_text(Text(name_ + " typing text (text)", location_, text_color_, "",
                     font_))
{
    SDL_Rect text_location = location_;
    text_location.x += 1;
    text_location.y += 1;
    text_location.w = 0;
    text_location.h -= 2;

    text.location = text_location;
    prompt_text.location = text_location;
    typing_text.location = text_location;

    typing_text.location.w = 0;

    text.updated = true;
    prompt_text.updated = true;
    typing_text.updated = true;
}

/*
 * Destructor.
 */
Text_Box::~Text_Box()
{}

/*
 * Render the text box. If the text has changed, first update the text. If the
 * text box is active, draw a flashing cursor within it.
 */
void Text_Box::render()
{
    // Render the background rectangle
    background.render();

    // The text box is active, render the typing buffer
    if(active)
    {
        typing_text.render();
    }
    // Otherwise
    else
    {
        // The current text is not empty, so render that
        if(text.text != "")
        {
            text.render();
        }
        // The current text is empty, but not the typing text, so render that
        else if(typing_text.text != "")
        {
            typing_text.render();
        }
        // Neither the current text or the typing text contain anything, so
        // render the prompt text
        else
        {
            prompt_text.render();
        }
    }

    // If the text box is active and the cursor is current supposed to be drawn,
    // draw the typing cursor
    if(active && CURSOR_ON)
    {
        SDL_SetRenderDrawColor(RENDERER, text_color->r, text_color->g,
                               text_color->b, text_color->a);
        SDL_RenderDrawLine(RENDERER,
                           typing_text.location.x + typing_text.location.w,
                           typing_text.location.y + 2,
                           typing_text.location.x + typing_text.location.w,
                           typing_text.location.y + 11);
    }
}

/*
 * Update the location of this text box.
 */
void Text_Box::update_location(SDL_Rect location_)
{
    location = location_;
    background.update_location(location_);
    text.update_location(location_);
    typing_text.update_location(location_);
    prompt_text.update_location(location_);
    text.updated = true;
    typing_text.updated = true;
    prompt_text.updated = true;
}

/*
 * Update the current text displayed in this text box.
 */
void Text_Box::update_current_text(std::string text_)
{
    text.update_text(text_);
}

/*
 * Take typed characters and add them to the typing buffer.
 */
void Text_Box::add_characters(char *ch)
{
    typing_text.text += ch;
    typing_text.updated = true;
}

/*
 * Remove the final character from the typing buffer.
 */
void Text_Box::delete_character()
{
    if(!typing_text.text.empty())
    {
        typing_text.text.pop_back();
    }

    typing_text.updated = true;
}

/*
 * Handle clicks. Make this text box active,
 * set the global active text box pointer,
 * set the SDL text input rect, and finally,
 * start SDL text input so that SDL_KEYDOWN and
 * SDL_KEYUP events will instead be reported
 * as SDL_TEXTINPUT until it is stopped.
 */
void Text_Box::clicked()
{
    if(!OBJECT_CLICKED)
    {
        std::cout << PINK_STDOUT << name << " clicked" << DEFAULT_STDOUT
                  << std::endl;

        if(!active)
        {
            active = true;
            ACTIVE_TEXT_BOX = this;
            SDL_SetTextInputRect(&location);
            SDL_StartTextInput();
        }
        OBJECT_CLICKED = true;
    }
}

/*
 * Stop SDL text input, send this graphics object to the function forwarder,
 * set the current text to be what is in the typing buffer, then set this text
 * box as updated. After that, deactivate this text box and clear the typing
 * buffer.
 */
void Text_Box::entered()
{
    if(graphics_object_type == TEXT_BOX)
    {
        std::cout << PINK_STDOUT << name << " entered" << DEFAULT_STDOUT
                  << std::endl;

        SDL_StopTextInput();
        text.text = typing_text.text;
        function_forwarder(this);
        text.updated = true;
        ACTIVE_TEXT_BOX = NULL;
        active = false;
        typing_text.text = "";
    }
    else
    {
        ((Input_Text_Box *) this)->entered();
    }
}

/*
 * Cancel input to this text box. Make it inactive, stop SDL text input, etc.
 */
void Text_Box::cancel_input()
{
    std::cout << PINK_STDOUT << name << " text input cancelled"
              << DEFAULT_STDOUT << std::endl;

    SDL_StopTextInput();
    text.updated = true;
    ACTIVE_TEXT_BOX = NULL;
    active = false;
}

/*
 * Set the colors of this text box.
 */
void Text_Box::set_colors(SDL_Color *color_, SDL_Color *text_color_)
{
    color = color_;
    background.set_color(color_);
    text.set_color(text_color_);
    prompt_text.set_color(text_color_);
    typing_text.set_color(text_color_);
    updated = true;
}

