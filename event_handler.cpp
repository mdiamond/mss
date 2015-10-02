/*
 * Matthew Diamond 2015
 * This file contains the functions that handle
 * events in the main thread.
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
    Page *p = &PAGES[CURRENT_PAGE];

    if(ACTIVE_TEXT_BOX != NULL && !ACTIVE_TEXT_BOX->was_clicked())
    {
        ACTIVE_TEXT_BOX->active = false;
        ACTIVE_TEXT_BOX = NULL;
    }

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
    while(SDL_PollEvent(e) != 0)
    {
        if(e->type == SDL_QUIT)
        {
            quit = true;
            break;
        }
        else if(e->type == SDL_KEYDOWN)
        {
            if(ACTIVE_TEXT_BOX != NULL &&
               e->key.keysym.sym == SDLK_RETURN)
            {
                ACTIVE_TEXT_BOX->text->current_text = TYPING_BUFFER;
                ACTIVE_TEXT_BOX->entered();
            }
            if(e->key.keysym.sym == SDLK_BACKSPACE)
            {
                if(!TYPING_BUFFER.empty())
                    TYPING_BUFFER.pop_back();
            }
        }
        else if(e->type == SDL_TEXTINPUT)
        {
            TYPING_BUFFER += e->text.text;
            ACTIVE_TEXT_BOX->typing_text->current_text = TYPING_BUFFER;
        }
        else if(e->type == SDL_MOUSEBUTTONDOWN ||
                e->type == SDL_MOUSEBUTTONUP ||
                e->type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                check_click();
            }
        }
    }
    return quit;
}
