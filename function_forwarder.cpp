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

// Included files
#include "main.hpp"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Waveform.hpp"
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
 * Given the name of a module, return a pointer to it if
 * it exists, or NULL if it doesn't.
 */
Module *find_module(string *string, vector<Module *> *modules)
{
    for(unsigned int i = 0; i < modules->size(); i ++)
    {
        if((*modules)[i]->name == *string)
            return (*modules)[i];
    }

    return NULL;
}

/*
 * Handle functions for the output module.
 */
void output_function_forwarder(Graphics_Object *g)
{
    Output *output = (Output *) g->parent;

    if(g->name == "on/off button (toggle_button)")
        output->toggle_audio_on();
    else if(g->name == "output input left (text_box)")
        output->set_input_l();
    else if(g->name == "output input right (text_box)")
        output->set_input_r();
}

bool can_floatify(string *string)
{
    if(string->empty())
        return false;
    if(!(isdigit((*string)[0]) || (*string)[0] == '-' ||
       (*string)[0] == '.'))
        return false;
    if(string->size() > 1)
        if(!(isdigit((*string)[1]) || 
           (*string)[1] == '.'))
            return false;
    return true;
}

/*
 * Handle functions for the oscillator module.
 */
void oscillator_function_forwarder(Graphics_Object *g)
{
    Oscillator *oscillator = (Oscillator *) g->parent;
    Text_Box *text_box;
    if(g->type == TEXT_BOX)
        text_box = (Text_Box *) g;

    if(g->name == "oscillator frequency (text_box)")
        oscillator->set_frequency();
    else if(g->name == "oscillator phase offset (text_box)")
        oscillator->set_phase_offset();
    else if(g->name == "oscillator pulse width (text_box)")
        oscillator->set_pulse_width();
    else if(g->name == "oscillator range low (text_box)")
        oscillator->set_range_low();
    else if(g->name == "oscillator range high (text_box)")
        oscillator->set_range_high();
}

/*
 * Handle functions for the graphics objects with
 * no parent module.
 */
void no_parent_function_forwarder(Graphics_Object *g)
{
    if(g->name == "add oscillator (button)")
    {
        string name = "Oscillator " + to_string(MODULES->size());
        Oscillator *oscillator = new Oscillator(&name, MODULES->size());
        MODULES->push_back(oscillator);
        MODULES_CHANGED = true;
        cout << "Added module " << name << endl;
    }
    else if(g->name == "previous page (button)")
    {
        if(CURRENT_PAGE > 0)
        {
            CURRENT_PAGE --;
            cout << "Switched to page " << CURRENT_PAGE << endl;
        }
    }
    else if(g->name == "next page (button)")
    {
        if(CURRENT_PAGE < PAGES->size() - 1)
        {
            CURRENT_PAGE ++;
            cout << "Switched to page " << CURRENT_PAGE << endl;
        }
    }
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
