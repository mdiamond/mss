/*
 * Matthew Diamond 2015
 * Functions for taking a graphics object as input, and determining
 * what should happen based on the type and naming of the graphics
 * object.
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
#include "image_processing.hpp"
#include "main.hpp"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"
#include "Module.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/VCA.hpp"

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
Module *find_module(string *name, vector<Module *> *modules)
{
    for(unsigned int i = 0; i < modules->size(); i ++)
    {
        if(modules->at(i)->name == *name)
            return modules->at(i);
    }

    return NULL;
}

Module *find_module_as_source(string *name, vector<Module *> *modules, Module *dst)
{
        Module *src = find_module(name, &MODULES);
        if(src == NULL)
            cout << RED_STDOUT << "Input could not be set, no such module" << DEFAULT_STDOUT << endl;
        else if(src == MODULES[0])
        {
            cout << RED_STDOUT << "The output module does not output any signals accessible within the context of this software"
                               << DEFAULT_STDOUT << endl;
            return NULL;
        }
        else if(src == dst)
        {
            cout << RED_STDOUT << "No module may output to itself" << DEFAULT_STDOUT << endl;
            return NULL;
        }

        return src;
}

/*
 * Return true if it is possible to turn this string into
 * a float. Return false otherwise.
 */
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

    if(g->name == "oscillator sin toggle (toggle button)")
        oscillator->switch_waveform(SIN);
    else if(g->name == "oscillator tri toggle (toggle button)")
        oscillator->switch_waveform(TRI);
    else if(g->name == "oscillator saw toggle (toggle button)")
        oscillator->switch_waveform(SAW);
    else if(g->name == "oscillator sqr toggle (toggle button)")
        oscillator->switch_waveform(SQR);
}

/*
 * Handle functions for the VCA module.
 */
void VCA_function_forwarder(Graphics_Object *g)
{
    // Vca *vca = (Vca *) g->parent;
}

/*
 * Handle functions for the mixer module.
 */
void mixer_function_forwarder(Graphics_Object *g)
{
    // Mixer *mixer = (Mixer *) g->parent;
}

/*
 * Add an oscillator.
 */
void add_oscillator()
{
    Oscillator *oscillator = new Oscillator();
    MODULES.push_back(oscillator);
    MODULES_CHANGED = true;
    cout << "Added module " << oscillator->name << endl;
}

/*
 * Add a VCA module.
 */
void add_VCA()
{
    Vca *vca = new Vca();
    MODULES.push_back(vca);
    MODULES_CHANGED = true;
    cout << "Added module " << vca->name << endl;
}

/*
 * Add a mixer module.
 */
void add_mixer()
{
    Mixer *mixer = new Mixer();
    MODULES.push_back(mixer);
    MODULES_CHANGED = true;
    cout << "Added module " << mixer->name << endl;
}

/*
 * Increment the current page.
 */
void next_page()
{
    if(CURRENT_PAGE > 0)
    {
        CURRENT_PAGE --;
        cout << "Switched to page " << CURRENT_PAGE << endl;
    }
}

/*
 * Decrement the current page.
 */
void previous_page()
{
    if(CURRENT_PAGE < PAGES.size() - 1)
    {
        CURRENT_PAGE ++;
        cout << "Switched to page " << CURRENT_PAGE << endl;
    }
}

/*
 * Handle functions for the graphics objects with
 * no parent module.
 */
void no_parent_function_forwarder(Graphics_Object *g)
{
    if(g->name == "add oscillator (button)")
        add_oscillator();
    else if(g->name == "add vca (button)")
        add_VCA();
    else if(g->name == "add mixer (button)")
        add_mixer();
    else if(g->name == "previous page (button)")
        next_page();
    else if(g->name == "next page (button)")
        previous_page();
}

/*
 * Whenever an object with an associated function is
 * clicked, call that function.
 */
void function_forwarder(Graphics_Object *g)
{
    if(g->type == RECT && SELECTING_SRC && CURRENT_INPUT_TOGGLE_BUTTON != NULL &&
       CURRENT_INPUT_TOGGLE_BUTTON->parent != g->parent)
    {
        CURRENT_INPUT_TOGGLE_BUTTON->parent->set(g->parent, CURRENT_INPUT_TOGGLE_BUTTON->input_num);
        SELECTING_SRC = false;
        reset_alphas();
        CURRENT_INPUT_TOGGLE_BUTTON->input_text_box->text.text = g->parent->name.substr(0, 3)+ " " + g->parent->name.substr(g->parent->name.find(" ") + 1);
        CURRENT_INPUT_TOGGLE_BUTTON = NULL;
    }

    // else if(((Input_Toggle_Button *) g)->b)
    //     ((Input_Toggle_Button *) g)->b = !((Input_Toggle_Button *) g)->b;

    else if(g->type != RECT)
    {
        if(g->parent == NULL)
            no_parent_function_forwarder(g);
        else if(g->parent->type == OUTPUT)
            output_function_forwarder(g);
        else if(g->parent->type == OSCILLATOR)
            oscillator_function_forwarder(g);
        else if(g->parent->type == VCA)
            VCA_function_forwarder(g);
        else if(g->parent->type == MIXER)
            mixer_function_forwarder(g);
    }
}
