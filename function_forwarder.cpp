/*
 * Matthew Diamond 2015
 * Functions for taking a graphics object as input, and determining
 * what should happen based on the type and naming of the graphics
 * object. The function forwarder is a way for graphics objects to
 * trigger functions that are not within the context of the class.
 * When a graphics object is clicked, it can call upon the
 * function forwarder to call the function that is explicitly
 * associated with the graphics object in question.
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
#include "function_forwarder.hpp"
#include "image_processing.hpp"
#include "load_patch.hpp"
#include "main.hpp"
#include "save_patch.hpp"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"
#include "Module.hpp"
#include "Modules/Adsr.hpp"
#include "Modules/Delay.hpp"
#include "Modules/Filter.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Multiplier.hpp"
#include "Modules/Noise.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Sah.hpp"

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Given the name of a module, return a pointer to it if
 * it exists, or NULL if it doesn't.
 */
Module *find_module(std::string *name, std::vector<Module *> *modules)
{
    for(unsigned int i = 0; i < modules->size(); i ++)
    {
        if(MODULES[i] != NULL)
            if(modules->at(i)->get_name() == *name
               || modules->at(i)->get_short_name() == *name)
                return modules->at(i);
    }

    return NULL;
}

/*
 * Given the name of a module, return a pointer to it if it exists,
 * is not the same module as the destination module, and is not the
 * output module. Otherwise, return NULL and print an error message.
 */
Module *find_module_as_source(std::string *name, std::vector<Module *> *modules, Module *dst)
{
        Module *src = find_module(name, &MODULES);
        if(src == NULL)
            std::cout << RED_STDOUT << "Input could not be set, no such module" << DEFAULT_STDOUT << std::endl;
        else if(src == MODULES[0])
        {
            std::cout << RED_STDOUT << "The output module does not output any signals accessible within the context of this software"
                               << DEFAULT_STDOUT << std::endl;
            return NULL;
        }
        else if(src == dst)
        {
            std::cout << RED_STDOUT << "No module may output to itself" << DEFAULT_STDOUT << std::endl;
            return NULL;
        }

        return src;
}

/*
 * Return true if it is possible to turn this string into
 * a float. Return false otherwise.
 */
bool can_floatify(std::string *string)
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
 * Handle functions for the ADSR module.
 */
void adsr_function_forwarder(Graphics_Object *g)
{
    Adsr *adsr = (Adsr *) g->parent;
    std::vector<std::string> possible_names;

    possible_names = {"reset current amplitude (button)"};

    if(g->name.size() > possible_names[0].size()
       && g->name.substr(g->name.size() - possible_names[0].size()) == possible_names[0])
        adsr->reset_current_amplitude();
}

/*
 * Handle functions for the delay module.
 */
void delay_function_forwarder(Graphics_Object *g)
{
    Delay *delay = (Delay *) g->parent;
    std::vector<std::string> possible_names;

    possible_names = {"reset buffer (button)"};

    if(g->name.size() > possible_names[0].size()
       && g->name.substr(g->name.size() - possible_names[0].size()) == possible_names[0])
        delay->reset_buffer();
}

/*
 * Handle functions for the filter module.
 */
void filter_function_forwarder(Graphics_Object *g)
{
    Filter *filter = (Filter *) g->parent;
    std::vector<std::string> possible_names;

    possible_names = {"lowpass toggle (toggle button)", "bandpass toggle (toggle button)",
                      "highpass toggle (toggle button)"};

    if(g->name.size() > possible_names[0].size()
            && g->name.substr(g->name.size() - 30) == possible_names[0])
        filter->switch_filter(LOWPASS);
    else if(g->name.size() > possible_names[1].size()
            && g->name.substr(g->name.size() - 31) == possible_names[1])
        filter->switch_filter(BANDPASS);
    else if(g->name.size() > possible_names[2].size()
            && g->name.substr(g->name.size() - 31) == possible_names[2])
        filter->switch_filter(HIGHPASS);
}

/*
 * Handle functions for the oscillator module.
 */
void oscillator_function_forwarder(Graphics_Object *g)
{
    Oscillator *oscillator = (Oscillator *) g->parent;
    std::vector<std::string> possible_names;

    possible_names = {"reset current phase (button)",
                      "sin toggle (toggle button)", "tri toggle (toggle button)",
                      "saw toggle (toggle button)", "sqr toggle (toggle button)"};

    if(g->name.size() > possible_names[0].size()
       && g->name.substr(g->name.size() - 28) == possible_names[0])
        oscillator->reset_current_phase();
    else if(g->name.size() > possible_names[1].size()
            && g->name.substr(g->name.size() - 26) == possible_names[1])
        oscillator->switch_waveform(SIN);
    else if(g->name.size() > possible_names[2].size()
            && g->name.substr(g->name.size() - 26) == possible_names[2])
        oscillator->switch_waveform(TRI);
    else if(g->name.size() > possible_names[3].size()
            && g->name.substr(g->name.size() - 26) == possible_names[3])
        oscillator->switch_waveform(SAW);
    else if(g->name.size() > possible_names[4].size()
            && g->name.substr(g->name.size() - 26) == possible_names[4])
        oscillator->switch_waveform(SQR);
}

/*
 * Handle functions for the output module.
 */
void output_function_forwarder(Graphics_Object *g)
{
    Output *output = (Output *) g->parent;

    if(g->name == "output on/off button (toggle_button)")
        output->toggle_audio_on();
}

/*
 * Handle functions for the sample and hold module.
 */
void sah_function_forwarder(Graphics_Object *g)
{
    Sah *sah = (Sah *) g->parent;

    if(g->name.substr(g->name.size() - 23) == " reset sampler (button)")
        sah->reset_sampler();
}

/*
 * Handle functions for the graphics objects with
 * no parent module.
 */
void no_parent_function_forwarder(Graphics_Object *g)
{
    std::vector<std::string> possible_names;

    possible_names = {"add adsr (button)", "add delay (button)", "add filter (button)",
                      "add mixer (button)", "add multiplier (button)", "add noise (button)",
                      "add oscillator (button)", "add sah (button)", "previous page (button)",
                      "next page (button)", "save patch (text box)", "load patch (text box)"};

    if(g->name == possible_names[0])
        create_module(ADSR);
    else if(g->name == possible_names[1])
        create_module(DELAY);
    else if(g->name == possible_names[2])
        create_module(FILTER);
    else if(g->name == possible_names[3])
        create_module(MIXER);
    else if(g->name == possible_names[4])
        create_module(MULTIPLIER);
    else if(g->name == possible_names[5])
        create_module(NOISE);
    else if(g->name == possible_names[6])
        create_module(OSCILLATOR);
    else if(g->name == possible_names[7])
        create_module(SAH);
    else if(g->name == possible_names[8])
        increment_page_number(-1);
    else if(g->name == possible_names[9])
        increment_page_number(1);
    else if(g->name == possible_names[10])
        save_patch(((Text_Box *) g)->text.text);
    else if(g->name == possible_names[11])
        load_patch(((Text_Box *) g)->text.text);
}

/*
 * Create a module of the type specified.
 */
void create_module(int type)
{
    Module *module;

    switch(type)
    {
        case ADSR: module = new Adsr(); break;
        case DELAY: module = new Delay(); break;
        case FILTER: module = new Filter(); break;
        case MIXER: module = new Mixer(); break;
        case MULTIPLIER: module = new Multiplier(); break;
        case NOISE: module = new Noise(); break;
        case OSCILLATOR: module = new Oscillator(); break;
        case SAH: module = new Sah(); break;
    }

    module->initialize_graphics_objects();

    bool push_on_back = false;
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] == NULL)
        {
            MODULES[i] = module;
            push_on_back = false;
            break;
        }

        if(i == MODULES.size() - 1)
            push_on_back = true;
    }

    if(push_on_back)
        MODULES.push_back(module);

    MODULES_CHANGED = true;
}

/*
 * Increment the current page by the amount specified.
 */
void increment_page_number(int num)
{
    unsigned int tmp = CURRENT_PAGE;

    tmp += num;

    if(tmp >= 0 && tmp < PAGES.size())
    {
        CURRENT_PAGE = tmp;
        std::cout << "Switched to page " << CURRENT_PAGE << std::endl;
    }
}

/**********************
 * FUNCTION FORWARDER *
 **********************/

/*
 * Whenever an object with an associated function is
 * clicked, call that function.
 */
void function_forwarder(Graphics_Object *g)
{
    // INPUT TOGGLE BUTTON HACK !!
    // If the type of the graphics object is a rectangle, we're currently selecting a source,
    // the current input toggle button is not NULL, and the parent of this graphics object
    // is not the same as the parent of the current input toggle button, set the input associated with
    // the current input toggle button to receive from the output of the parent of this graphics object
    if(g->type == RECT && SELECTING_SRC && CURRENT_INPUT_TOGGLE_BUTTON != NULL &&
       CURRENT_INPUT_TOGGLE_BUTTON->parent != g->parent)
    {
        CURRENT_INPUT_TOGGLE_BUTTON->parent->set(g->parent, CURRENT_INPUT_TOGGLE_BUTTON->input_num);
        CURRENT_INPUT_TOGGLE_BUTTON->b = true;
        SELECTING_SRC = false;
        CURRENT_INPUT_TOGGLE_BUTTON->input_text_box->update_current_text(g->parent->name.substr(0, 3)
                                                                         + " " + g->parent->name.substr(g->parent->name.find(" ") + 1));
        CURRENT_INPUT_TOGGLE_BUTTON = NULL;
    }
    // If the input toggle button hack does not kick in, first check to see
    // the graphics object is a remove module button, and if so, remove that module
    else if(g->name.size() > 22
            && g->name.substr(g->name.size() - 22) == "remove module (button)"
            && g->parent->get_name() != "output")
    {
        for(unsigned int i = 0; i < MODULES.size(); i ++)
            if(MODULES[i] != NULL && MODULES[i] == g->parent)
                MODULES[i] = NULL;

        delete g->parent;
    }
    // Next check for all possible graphics object parents, and do something depending
    // on the module type of the parent
    else if(g->parent == NULL)
        no_parent_function_forwarder(g);
    else if(g->parent->type == ADSR)
        adsr_function_forwarder(g);
    else if(g->parent->type == DELAY)
        delay_function_forwarder(g);
    else if(g->parent->type == FILTER)
        filter_function_forwarder(g);
    else if(g->parent->type == OSCILLATOR)
        oscillator_function_forwarder(g);
    else if(g->parent->type == OUTPUT)
        output_function_forwarder(g);
    else if(g->parent->type == SAH)
        sah_function_forwarder(g);
}
