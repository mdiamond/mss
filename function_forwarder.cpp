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
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "function_forwarder.hpp"
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "load_patch.hpp"
#include "main.hpp"
#include "module_utils.hpp"
#include "save_patch.hpp"

// Included modules classes
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

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

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
        create_module(Module::ADSR);
    else if(g->name == possible_names[1])
        create_module(Module::DELAY);
    else if(g->name == possible_names[2])
        create_module(Module::FILTER);
    else if(g->name == possible_names[3])
        create_module(Module::MIXER);
    else if(g->name == possible_names[4])
        create_module(Module::MULTIPLIER);
    else if(g->name == possible_names[5])
        create_module(Module::NOISE);
    else if(g->name == possible_names[6])
        create_module(Module::OSCILLATOR);
    else if(g->name == possible_names[7])
        create_module(Module::SAH);
    else if(g->name == possible_names[8])
        increment_page_number(-1);
    else if(g->name == possible_names[9])
        increment_page_number(1);
    else if(g->name == possible_names[10])
        save_patch(((Text_Box *) g)->text.text);
    else if(g->name == possible_names[11])
        load_patch(((Text_Box *) g)->text.text);
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
    // If the graphics object has no parent, find out what to do with it
    if(g->parent == NULL)
        no_parent_function_forwarder(g);
}

