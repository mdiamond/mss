/*
 * Matthew Diamond 2015
 * Member functions for the Text Box class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <sstream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"

// Included files
#include "graphics_config.hpp"

// Included graphics classes
#include "Graphics_Listener.hpp"
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text_Box.hpp"

/*****************************
 * TEXT BOX MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Text_Box::Text_Box(std::string name_, SDL_Rect location_, SDL_Color color_,
                   SDL_Color text_color_, std::string prompt_text_,
                   Graphics_Listener *listener_) :
    Graphics_Object(name_, TEXT_BOX, listener_, location_, color_),
    text_color(text_color_),
    active(false), is_float(false),
    background(Rect(name_ + " background rect", location_, color_,
                    NULL)),
    text(Text(name_ + " idle text", location_, text_color_, "")),
    prompt_text(Text(name_ + " text", location_, text_color_,
                     prompt_text_)),
    typing_text(Text(name_ + " typing text", location_, text_color_, ""))
{
    SDL_Rect text_location = location_;
    text_location.x += 1;
    text_location.y += 1;
    text_location.w = 0;
    text_location.h -= 2;

    text.location = text_location;
    prompt_text.location = text_location;
    typing_text.location = text_location;

    text.max_width = location.w - 2;
    prompt_text.max_width = location.w - 2;
    typing_text.max_width = location.w - 2;

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
        // Neither the current text nor the typing text contain anything, so
        // render the prompt text
        else
        {
            prompt_text.render();
        }
    }

    // If the text box is active and the cursor is currently supposed to be
    // drawn, draw the typing cursor
    if(active && SHOW_TYPING_CURSOR)
    {
        SDL_SetRenderDrawColor(RENDERER, text_color.r, text_color.g,
                               text_color.b, text_color.a);

        SDL_RenderDrawLine(RENDERER,
                           typing_text.location.x + typing_text.location.w,
                           typing_text.location.y + 1,
                           typing_text.location.x + typing_text.location.w,
                           typing_text.location.y + 7);
    }
}

/*
 * Update the location of this text box.
 */
void Text_Box::update_location(SDL_Rect location_)
{
    location = location_;
    background.update_location(location_);
    location_.x += 1;
    text.update_location(location_);
    typing_text.update_location(location_);
    prompt_text.update_location(location_);
    updated = true;
}

/*
 * Update the current text displayed in this text box.
 */
void Text_Box::update_current_text(std::string text_)
{
    text.update_text(text_);
    updated = true;
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
 * Handle clicks. Make this text box active, set the global active text box
 * pointer, set the SDL text input rect, and finally, start SDL text input so
 * that SDL_KEYDOWN and SDL_KEYUP events will instead be reported as
 * SDL_TEXTINPUT until it is stopped. Return true if action is taken, false
 * otherwise.
 */
bool Text_Box::clicked()
{
    if(!active)
    {
        active = true;
        ACTIVE_TEXT_BOX = this;
        SDL_SetTextInputRect(&typing_text.location);
        SDL_StartTextInput();
        return true;
    }
    return false;
}

/*
 * Stop SDL text input, update the current text to be what was entered, update
 * the typing text to be an empty string again, store a float version of the
 * entered text if possible, set this text box to be inactive, then invoke
 * the handle_event function of the listener.
 */
void Text_Box::entered()
{
    SDL_StopTextInput();
    text.update_text(typing_text.text);
    typing_text.update_text("");
    store_float();
    ACTIVE_TEXT_BOX = nullptr;
    active = false;

    if(listener != nullptr)
    {
        listener->handle_event(this);
    }
}

/*
 * Cancel input to this text box. Make it inactive, stop SDL text input, etc.
 */
void Text_Box::cancel_input()
{
    SDL_StopTextInput();
    text.updated = true;
    ACTIVE_TEXT_BOX = nullptr;
    active = false;
}

/*
 * Set the colors of this text box.
 */
void Text_Box::set_colors(SDL_Color color_, SDL_Color text_color_)
{
    color = color_;
    background.set_color(color_);
    text.set_color(text_color_);
    prompt_text.set_color(text_color_);
    typing_text.set_color(text_color_);
    updated = true;
}

/*
 * Check if the string entered is a float. Update bool is_float accordingly,
 * then store the float if applicable. Solution for converting a string to a
 * float adapted from here:
 * https://stackoverflow.com/questions/447206/c-isfloat-function
 */
void Text_Box::store_float()
{
    std::istringstream stream(text.text);
    stream >> std::noskipws >> as_float;
    if(stream.eof() && !stream.fail())
    {
        is_float = true;
    }
    else
    {
        is_float = false;
    }
}

