/*
 * Matthew Diamond 2015
 * This file contains the functions that handle
 * events in the main thread.
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
#include "function_forwarder.hpp"
#include "main.hpp"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Page.hpp"

using namespace std;

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * For each graphics object on the current page, 
 * check if it has been clicked. If so, call its
 * clicked() function. Return true if something
 * was clicked, false if nothing was clicked.
 */
bool check_click()
{
    bool clicked = false;
    Page *p = (*PAGES)[CURRENT_PAGE];

    // If there is an active text box, and it is not what was clicked,
    // make the text box inactive
    if(ACTIVE_TEXT_BOX != NULL && !ACTIVE_TEXT_BOX->was_clicked())
    {
        ACTIVE_TEXT_BOX->active = false;
        ACTIVE_TEXT_BOX = NULL;
    }

    // If the current page was clicked, call its clicked() function
    if(p->was_clicked())
        p->clicked();

    return clicked;
}

/*
 * Handle SDL_KEYDOWN events.
 */
void keydown_event(SDL_Event *e)
{
    // If a keyboard combination for creating a module or switching
    // pages is entered, handle that
    if(e->key.keysym.sym == SDLK_1)
    {
        if(e->key.keysym.mod & KMOD_LCTRL)
            add_oscillator();
    }
    else if(e->key.keysym.sym == SDLK_LEFTBRACKET)
    {
        if(e->key.keysym.mod & KMOD_LCTRL)
            next_page();
    }
    else if(e->key.keysym.sym == SDLK_RIGHTBRACKET)
    {
        if(e->key.keysym.mod & KMOD_LCTRL)
            previous_page();
    }
    // If that key is the return key, and there is an active
    // text box, call its entered() function and pass it the
    // text that's been recorded from the keyboard previously
    else if(ACTIVE_TEXT_BOX != NULL && e->key.keysym.sym == SDLK_RETURN)
    {
        ACTIVE_TEXT_BOX->entered();
    }

    // If that key is backspace, just remove a character
    // from the typing buffer
    else if(e->key.keysym.sym == SDLK_BACKSPACE && ACTIVE_TEXT_BOX != NULL)
    {
        ACTIVE_TEXT_BOX->delete_character();
    }
}

/*
 * Handle SDL_MOUSEBUTTONDOWN and SDL_MOUSEMOTION
 * events.
 */
void mouse_event(SDL_Event *e)
{
    MOUSE_X = e->motion.x;
    MOUSE_Y = e->motion.y;
    if(e->type == SDL_MOUSEBUTTONDOWN)
        check_click();
}

/*****************
 * EVENT HANDLER *
 *****************/

/*
 * Handle events. Return true if SDL_QUIT event
 * is received.
 */
bool event_handler(SDL_Event *e)
{
    bool quit = false;

    // While there are events remaining to handle
    while(SDL_PollEvent(e) != 0)
    {
        // If the event is SQL_QUIT, just set quit to true, break
        // out of the event handler loop, and the calling function
        // will terminate the program
        if(e->type == SDL_QUIT)
            quit = true;

        // Otherwise, if a keyboard key is pressed...
        else if(e->type == SDL_KEYDOWN)
            keydown_event(e);

        // If neither an SDL_QUIT or SDL_KEYDOWN event has been received,
        // but a SDL_TEXTINPUT event has been received, add the typed text
        // to the typing buffer and pass it to the active text box (it is
        // impossible to receive an event of this type if no text box is
        // current active)
        else if(e->type == SDL_TEXTINPUT && ACTIVE_TEXT_BOX != NULL)
            ACTIVE_TEXT_BOX->typed(e->text.text);

        // If none of the above events have been received, but any kind of
        // mouse event has been received, get the mouse coordinates, and
        // if the event is an SDL_MOUSEBUTTONDOWN, check for clicked objects
        else if(e->type == SDL_MOUSEBUTTONDOWN ||
                e->type == SDL_MOUSEMOTION)
            mouse_event(e);
    }

    return quit;
}
