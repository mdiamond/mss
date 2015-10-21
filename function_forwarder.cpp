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
        if((*modules)[i]->name == *name)
            return (*modules)[i];
    }

    return NULL;
}

Module *find_module_as_source(string *name, vector<Module *> *modules, Module *dst)
{
        Module *src = find_module(name, MODULES);
        if(src == NULL)
            cout << RED_STDOUT << "Input could not be set, no such module" << DEFAULT_STDOUT << endl;
        else if(src == (*MODULES)[0])
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
    Text_Box *text_box = NULL;
    Module *src = NULL;

    if(g->type == TEXT_BOX)
    {
        text_box = (Text_Box *) g;

        if(can_floatify(&text_box->typing_text->text))
        {
            cout << RED_STDOUT << "The output module can only take other modules' output as input" << DEFAULT_STDOUT << endl;
            return;
        }

        src = find_module_as_source(&text_box->typing_text->text, MODULES, g->parent);
        if(src == NULL)
            return;

        if(g->name == "output input left (text box)")
            output->set_input_l(src);
        else if(g->name == "output input right (text box)")
            output->set_input_r(src);
    }
    else if(g->name == "on/off button (toggle_button)")
        output->toggle_audio_on();
}

/*
 * Handle functions for the oscillator module.
 */
void oscillator_function_forwarder(Graphics_Object *g)
{
    Oscillator *oscillator = (Oscillator *) g->parent;
    Text_Box *text_box = NULL;
    Module *src = NULL;
    float val = 0;

    if(g->type == TEXT_BOX)
    {
        text_box = (Text_Box *) g;

        if(can_floatify(&text_box->typing_text->text))
        {
            val = stof(text_box->typing_text->text.c_str());

            if(g->name == "oscillator frequency (text box)")
                oscillator->set_frequency(val);
            else if(g->name == "oscillator phase offset (text box)")
                oscillator->set_phase_offset(val);
            else if(g->name == "oscillator pulse width (text box)")
                oscillator->set_pulse_width(val);
            else if(g->name == "oscillator range low (text box)")
                oscillator->set_range_low(val);
            else if(g->name == "oscillator range high (text box)")
                oscillator->set_range_high(val);
        }
        else
        {
            src = find_module_as_source(&text_box->typing_text->text, MODULES, g->parent);
            if(src == NULL)
                return;

            else if(g->name == "oscillator frequency (text box)")
                oscillator->set_frequency(src);
            else if(g->name == "oscillator phase offset (text box)")
                oscillator->set_phase_offset(src);
            else if(g->name == "oscillator pulse width (text box)")
                oscillator->set_pulse_width(src);
            else if(g->name == "oscillator range low (text box)")
                oscillator->set_range_low(src);
            else if(g->name == "oscillator range high (text box)")
                oscillator->set_range_high(src);
        }
    }
    else
    {
        if(g->name == "oscillator sin toggle (toggle button)")
            oscillator->switch_waveform(SIN);
        else if(g->name == "oscillator tri toggle (toggle button)")
            oscillator->switch_waveform(TRI);
        else if(g->name == "oscillator saw toggle (toggle button)")
            oscillator->switch_waveform(SAW);
        else if(g->name == "oscillator sqr toggle (toggle button)")
            oscillator->switch_waveform(SQR);
        // else if(g->name == "oscillator frequency input (toggle button)")

    }
}

/*
 * Handle functions for the VCA module.
 */
void VCA_function_forwarder(Graphics_Object *g)
{
    Vca *vca = (Vca *) g->parent;
    Text_Box *text_box = NULL;
    Module *src = NULL;
    float val = 0;

    if(g->type == TEXT_BOX)
    {
        text_box = (Text_Box *) g;

        if(can_floatify(&text_box->typing_text->text))
        {
            val = stof(text_box->typing_text->text.c_str());

            if(g->name == "vca signal (text box)" || g->name == "vca cv (text box)")
                cout << RED_STDOUT << "The VCA module's top two input fields must be other modules"
                     << DEFAULT_STDOUT << endl;
            else if(g->name == "vca cv amount (text box)")
                vca->set_cv_amount(val);
        }
        else
        {
            src = find_module_as_source(&text_box->typing_text->text, MODULES, g->parent);
            if(src == NULL)
                return;

            else if(g->name == "vca signal (text box)")
                vca->set_signal(src);
            else if(g->name == "vca cv (text box)")
                vca->set_cv(src);
            else if(g->name == "vca cv amount (text box)")
                vca->set_cv_amount(src);
        }
    }
}

/*
 * Handle functions for the mixer module.
 */
void mixer_function_forwarder(Graphics_Object *g)
{
    Mixer *mixer = (Mixer *) g->parent;
    Text_Box *text_box = NULL;
    Module *src = NULL;
    float val = 0;

    if(g->type == TEXT_BOX)
    {
        text_box = (Text_Box *) g;

        if(can_floatify(&text_box->typing_text->text))
        {
            val = stof(text_box->typing_text->text.c_str());

            if(g->name == "mixer signal 1 (text box)" || g->name == "mixer signal 2 (text box)" ||
               g->name == "mixer signal 3 (text box)" || g->name == "mixer signal 4 (text box)" ||
               g->name == "mixer signal 5 (text box)" || g->name == "mixer signal 6 (text box)" ||
               g->name == "mixer signal 7 (text box)" || g->name == "mixer signal 8 (text box)")
                cout << RED_STDOUT << "The mixer modules left hand input fields must be other modules"
                     << DEFAULT_STDOUT << endl;
            else if(g->name == "mixer signal 1 multiplier (text box)")
                mixer->set_signal_multiplier(val, 1);
            else if(g->name == "mixer signal 2 multiplier (text box)")
                mixer->set_signal_multiplier(val, 2);
            else if(g->name == "mixer signal 3 multiplier (text box)")
                mixer->set_signal_multiplier(val, 3);
            else if(g->name == "mixer signal 4 multiplier (text box)")
                mixer->set_signal_multiplier(val, 4);
            else if(g->name == "mixer signal 5 multiplier (text box)")
                mixer->set_signal_multiplier(val, 5);
            else if(g->name == "mixer signal 6 multiplier (text box)")
                mixer->set_signal_multiplier(val, 6);
            else if(g->name == "mixer signal 7 multiplier (text box)")
                mixer->set_signal_multiplier(val, 7);
            else if(g->name == "mixer signal 8 multiplier (text box)")
                mixer->set_signal_multiplier(val, 8);
        }
        else
        {
            src = find_module_as_source(&text_box->typing_text->text, MODULES, g->parent);
            if(src == NULL)
                return;

            else if(g->name == "mixer signal 1 (text box)")
                mixer->set_signal(src, 1);
            else if(g->name == "mixer signal 1 multiplier (text box)")
                mixer->set_signal_multiplier(src, 1);
            else if(g->name == "mixer signal 2 (text box)")
                mixer->set_signal(src, 2);
            else if(g->name == "mixer signal 2 multiplier (text box)")
                mixer->set_signal_multiplier(src, 2);
            else if(g->name == "mixer signal 3 (text box)")
                mixer->set_signal(src, 3);
            else if(g->name == "mixer signal 3 multiplier (text box)")
                mixer->set_signal_multiplier(src, 3);
            else if(g->name == "mixer signal 4 (text box)")
                mixer->set_signal(src, 4);
            else if(g->name == "mixer signal 4 multiplier (text box)")
                mixer->set_signal_multiplier(src, 4);
            else if(g->name == "mixer signal 5 (text box)")
                mixer->set_signal(src, 5);
            else if(g->name == "mixer signal 5 multiplier (text box)")
                mixer->set_signal_multiplier(src, 5);
            else if(g->name == "mixer signal 6 (text box)")
                mixer->set_signal(src, 6);
            else if(g->name == "mixer signal 6 multiplier (text box)")
                mixer->set_signal_multiplier(src, 6);
            else if(g->name == "mixer signal 7 (text box)")
                mixer->set_signal(src, 7);
            else if(g->name == "mixer signal 7 multiplier (text box)")
                mixer->set_signal_multiplier(src, 7);
            else if(g->name == "mixer signal 8 (text box)")
                mixer->set_signal(src, 8);
            else if(g->name == "mixer signal 8 multiplier (text box)")
                mixer->set_signal_multiplier(src, 8);
        }
    }
}

/*
 * Add an oscillator.
 */
void add_oscillator()
{
    string name = "Oscillator " + to_string(MODULES->size());
    Oscillator *oscillator = new Oscillator(name, MODULES->size());
    MODULES->push_back(oscillator);
    MODULES_CHANGED = true;
    cout << "Added module " << name << endl;
}

/*
 * Add a VCA module.
 */
void add_VCA()
{
    string name = "VCA " + to_string(MODULES->size());
    Vca *vca = new Vca(name, MODULES->size());
    MODULES->push_back(vca);
    MODULES_CHANGED = true;
    cout << "Added module " << name << endl;
}

/*
 * Add a mixer module.
 */
void add_mixer()
{
    string name = "Mixer " + to_string(MODULES->size());
    Mixer *mixer = new Mixer(name, MODULES->size());
    MODULES->push_back(mixer);
    MODULES_CHANGED = true;
    cout << "Added module " << name << endl;
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
    if(CURRENT_PAGE < PAGES->size() - 1)
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

vector<float> **get_dst_buffer(Graphics_Object *g)
{
    Output *output;
    Oscillator *oscillator;
    Vca *vca;

    switch(g->parent->type)
    {
        case OUTPUT:
            output = (Output *) g->parent;
            break;
        case OSCILLATOR:
            oscillator = (Oscillator *) g->parent;
            break;
        case VCA:
            vca = (Vca *) g->parent;
    }

    switch(g->name)
    {
        case "oscillator frequency input (toggle button)":
            return &oscillator->frequency_input;
        case "oscillator phase offset input (toggle button)":
            return &oscillator->phase_offset_input;
        case "oscillator pulse width input (toggle button)":
            return &oscillator->pulse_width_input;
        case "oscillator range low input (toggle button)":
            return &oscillator->range_low_input;
        case "oscillator range high input (toggle button)":
            return &oscillator->range_high_input;
        case "vca signal input (toggle button)":
            return &vca->signal_input;
        case "vca cv input (toggle button)":
            return &vca->cv_input;
        case "vca cv amount input (toggle button)":
            return &vca->cv_amount_input;
    }

    return NULL;
}

/*
 * Whenever an object with an associated function is
 * clicked, call that function.
 */
void function_forwarder(Graphics_Object *g)
{
    if(g->type == RECT)
    {
        CURRENT_SRC = g->parent;
        CURRENT_SRC = g->parent->output;

        CURRENT_SRC = NULL;
        CURRENT_SRC_BUFFER = NULL;
        CURRENT_DST = NULL;
        CURRENT_DST_BUFFER = NULL;
    }

    if(g->type == TOGGLE_BUTTON && g->name.substr(g->name.size() - 21) == "input (toggle button)")
    {
        SELECTING_SRC = !SELECTING_SRC;
        if(SELECTING_SRC == false)
            reset_alphas();
        else
            CURRENT_DST = g->parent;

        return;
    }

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
