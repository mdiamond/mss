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

// Included files
#include "Function_Forwarder.hpp"
#include "graphics_object_utils.hpp"
#include "load_patch.hpp"
#include "module_utils.hpp"
#include "save_patch.hpp"

// Included modules classes
#include "Module.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Text_Box.hpp"

/***************************************
 * FUNCTION FORWARDER MEMBER FUNCTIONS *
 ***************************************/

Function_Forwarder::Function_Forwarder()
{}

Function_Forwarder::~Function_Forwarder()
{}

/*
 * Catchall handler for graphics objects that don't belong to any module.
 */
bool Function_Forwarder::handle_event(Graphics_Object *g)
{
    if(g->name == possible_names[0])
    {
        create_module(Module::ADSR);
    }
    else if(g->name == possible_names[1])
    {
        create_module(Module::DELAY);
    }
    else if(g->name == possible_names[2])
    {
        create_module(Module::FILTER);
    }
    else if(g->name == possible_names[3])
    {
        create_module(Module::MIXER);
    }
    else if(g->name == possible_names[4])
    {
        create_module(Module::MULTIPLIER);
    }
    else if(g->name == possible_names[5])
    {
        create_module(Module::NOISE);
    }
    else if(g->name == possible_names[6])
    {
        create_module(Module::OSCILLATOR);
    }
    else if(g->name == possible_names[7])
    {
        create_module(Module::SAH);
    }
    else if(g->name == possible_names[8])
    {
        increment_page_number(-1);
    }
    else if(g->name == possible_names[9])
    {
        increment_page_number(1);
    }
    else if(g->name == possible_names[10])
    {
        save_patch(((Text_Box *) g)->text.text);
    }
    else if(g->name == possible_names[11])
    {
        load_patch(((Text_Box *) g)->text.text);
    }
    return true;
}

