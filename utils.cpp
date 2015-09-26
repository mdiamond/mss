/*
 * Matthew Diamond 2015
 * Functions for accomplishing miscellaneous tasks.
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
#include "SDL2/SDL_ttf.h"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

using namespace std;

/**********************
 * FUNCTION FORWARDER *
 **********************/

/*
 * The function forwarder is a way for graphics objects to
 * trigger functions within the modules that contain them.
 * When a graphics object is clicked, it can call upon the
 * function forwarder to call the function that is explicitly
 * associated with the graphics object in question.
 */

/*
 * Handle functions for the output module.
 */
void output_function_forwarder(Graphics_Object *g)
{
    Output *output = (Output *) g->parent;
    if(g->name == "on/off button (toggle_button)")
        output->toggle_audio_on();
}

/*
 * Handle functions for the oscillator module.
 */
void oscillator_function_forwarder(Graphics_Object *g)
{
    Oscillator *oscillator = (Oscillator *) g->parent;
    if(g->name == "oscillator frequency (text_box)")
    {
        Text_Box *frequency_text_box = (Text_Box *) g;
        SDL_LockAudio();
        oscillator->audio.frequency = stoi(frequency_text_box->text->current_text.c_str());
        SDL_UnlockAudio();
    }
}

/*
 * Handle functions for the graphics objects with
 * no parent module.
 */
void no_parent_function_forwarder(Graphics_Object *g)
{

}

/*
 * Whenever an object with an associated function is
 * clicked, call that function.
 */
void function_forwarder(Graphics_Object *g)
{
    if(g->parent == NULL)
        no_parent_function_forwarder(g);
    else if(g->parent->type == OUTPUT)
        output_function_forwarder(g);
    else if(g->parent->type == OSCILLATOR)
        oscillator_function_forwarder(g);
}
