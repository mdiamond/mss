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
        {
            quit = true;
            break;
        }

        // Otherwise, if a keyboard key is pressed...
        else if(e->type == SDL_KEYDOWN)
        {
            // If that key is the return key, and there is an active
            // text box, call its entered() function and pass it the
            // text that's been recorded from the keyboard previously
            if(ACTIVE_TEXT_BOX != NULL &&
               e->key.keysym.sym == SDLK_RETURN)
            {
                ACTIVE_TEXT_BOX->text->current_text = TYPING_BUFFER;
                ACTIVE_TEXT_BOX->entered();
            }

            // If that key is backspace, just remove a character
            // from the typing buffer
            else if(e->key.keysym.sym == SDLK_BACKSPACE && ACTIVE_TEXT_BOX != NULL)
            {
                if(!TYPING_BUFFER.empty())
                    TYPING_BUFFER.pop_back();
                ACTIVE_TEXT_BOX->typing_text->current_text = TYPING_BUFFER;
            }
        }

        // If neither an SDL_QUIT or SDL_KEYDOWN event has been received,
        // but a SDL_TEXTINPUT event has been received, add the typed text
        // to the typing buffer and pass it to the active text box (it is
        // impossible to receive an event of this type if no text box is
        // current active)
        else if(e->type == SDL_TEXTINPUT && ACTIVE_TEXT_BOX != NULL)
        {
            TYPING_BUFFER += e->text.text;
            ACTIVE_TEXT_BOX->typing_text->current_text = TYPING_BUFFER;
        }

        // If none of the above events have been received, but any kind of
        // mouse event has been received, get the mouse coordinates, and
        // if the event is an SDL_MOUSEBUTTONDOWN, check for clicked objects
        else if(e->type == SDL_MOUSEBUTTONDOWN ||
                e->type == SDL_MOUSEBUTTONUP ||
                e->type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
            if(e->type == SDL_MOUSEBUTTONDOWN)
                check_click();
        }
    }

    return quit;
}
