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
    {
        output->toggle_audio_on();
        cout << "Audio toggled" << endl;
    }
    if(g->name == "output input left (text_box)")
    {
        Text_Box *text_box = (Text_Box *) g;
        Module *module = find_module(&text_box->text->current_text, &MODULES);
        if(find_module(&module->name, &output->depends) == NULL)
            output->depends.push_back(module);
        output->audio.input_l = module->output;
        cout << "Output left is now coming from " << module->name << endl;
    }
    else if(g->name == "output input right (text_box)")
    {
        Text_Box *text_box = (Text_Box *) g;
        Module *module = find_module(&text_box->text->current_text, &MODULES);
        if(find_module(&module->name, &output->depends) == NULL)
            output->depends.push_back(module);
        output->audio.input_r = module->output;
        cout << "Output right is now coming from " << module->name << endl;
    }
}

bool can_floatify(string *string)
{
    return isdigit((*string)[0]) || (*string)[0] == '-' ||
           (*string)[0] == '.';
}

/*
 * Handle functions for the oscillator module.
 */
void oscillator_function_forwarder(Graphics_Object *g)
{
    Oscillator *oscillator = (Oscillator *) g->parent;
    Text_Box *text_box;
    Button *button;
    if(g->type == TEXT_BOX)
        text_box = (Text_Box *) g;
    else if(g->type == BUTTON)
        button = (Button *) g;

    if(g->name == "oscillator frequency (text_box)")
    {
        if(can_floatify(&text_box->text->current_text))
        {
            oscillator->audio.frequency = stof(text_box->text->current_text.c_str());
            oscillator->audio.live_frequency = false;
            cout << oscillator->name << " frequency changed to " <<  oscillator->audio.frequency << endl;
        }
        else
        {
            Module *module = find_module(&text_box->text->current_text, &MODULES);
            if(find_module(&module->name, &oscillator->depends) == NULL)
                oscillator->depends.push_back(module);
            oscillator->audio.input_frequency = module->output;
            oscillator->audio.live_frequency = true;
            cout << oscillator->name << " frequency is now coming from " << module->name << endl;
        }
    }
    else if(g->name == "oscillator phase offset (text_box)")
    {
        if(can_floatify(&text_box->text->current_text))
        {
            oscillator->audio.phase_offset = stof(text_box->text->current_text.c_str());
            oscillator->audio.live_phase_offset = false;
            cout << oscillator->name << " phase offset changed to " <<  oscillator->audio.phase_offset << endl;
        }
        else
        {
            Module *module = find_module(&text_box->text->current_text, &MODULES);
            if(find_module(&module->name, &oscillator->depends) == NULL)
                oscillator->depends.push_back(module);
            oscillator->audio.input_phase_offset = module->output;
            oscillator->audio.live_phase_offset = true;
            cout << oscillator->name << " phase offset is now coming from " << module->name << endl;
        }
    }
    else if(g->name == "oscillator pulse width (text_box)")
    {
        if(can_floatify(&text_box->text->current_text))
        {
            oscillator->audio.pulse_width = stof(text_box->text->current_text.c_str());
            oscillator->audio.live_pulse_width = false;
            cout << oscillator->name << " pulse width changed to " <<  oscillator->audio.pulse_width << endl;
        }
        else
        {
            Module *module = find_module(&text_box->text->current_text, &MODULES);
            if(find_module(&module->name, &oscillator->depends) == NULL)
                oscillator->depends.push_back(module);
            oscillator->audio.input_pulse_width = module->output;
            oscillator->audio.live_pulse_width = true;
            cout << oscillator->name << " pulse width is now coming from " << module->name << endl;
        }
    }
    else if(g->name == "oscillator range low (text_box)")
    {
        if(can_floatify(&text_box->text->current_text))
        {
            oscillator->audio.range_low = stof(text_box->text->current_text.c_str());
            oscillator->audio.live_range_low = false;
            cout << oscillator->name << " range low changed to " <<  oscillator->audio.range_low << endl;
        }
        else
        {
            Module *module = find_module(&text_box->text->current_text, &MODULES);
            if(find_module(&module->name, &oscillator->depends) == NULL)
                oscillator->depends.push_back(module);
            oscillator->audio.input_range_low = module->output;
            oscillator->audio.live_range_low = true;
            cout << oscillator->name << " range low is now coming from " << module->name << endl;
        }
    }
    else if(g->name == "oscillator range high (text_box)")
    {
        if(can_floatify(&text_box->text->current_text))
        {
            oscillator->audio.range_high = stof(text_box->text->current_text.c_str());
            oscillator->audio.live_range_high = false;
            cout << oscillator->name << " range high changed to " <<  oscillator->audio.range_high << endl;
        }
        else
        {
            Module *module = find_module(&text_box->text->current_text, &MODULES);
            if(find_module(&module->name, &oscillator->depends) == NULL)
                oscillator->depends.push_back(module);
            oscillator->audio.input_range_high = module->output;
            oscillator->audio.live_range_high = true;
            cout << oscillator->name << " range high is now coming from " << module->name << endl;
        }
    }
}

/*
 * Handle functions for the graphics objects with
 * no parent module.
 */
void no_parent_function_forwarder(Graphics_Object *g)
{
    if(g->name == "add oscillator (button)")
    {
        string name = "Oscillator " + to_string(MODULES.size());
        Oscillator *oscillator = new Oscillator(&name, MODULES.size());
        MODULES.push_back(oscillator);
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
        if(CURRENT_PAGE < PAGES.size() - 1)
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
