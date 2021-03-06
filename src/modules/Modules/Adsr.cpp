/*
 * Matthew Diamond 2015
 * Member functions for the ADSR class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Adsr.hpp"

/*************************
 * ADSR MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Adsr::Adsr() :
    Module(ADSR),
    current_amplitude(0), adsr_stage(ADSR_A_STAGE)
{
    inputs[ADSR_A].val = 500;
    inputs[ADSR_D].val = 500;
    inputs[ADSR_S].val = 1;
    inputs[ADSR_R].val = 500;
}

/*
 * Destructor.
 */
Adsr::~Adsr()
{}

/*
 * Process all dependencies, then fill the output buffer with a waveform given
 * the data contained within this class and the audio device information.
 */
void Adsr::process()
{
    // Process any dependencies
    process_dependencies();

    // Calculate an amplitude for each sample
    for(unsigned short i = 0; i < out.size(); i ++)
    {
        // Update parameters
        update_input_vals(i);

        if(!inputs[ADSR_NOTE].live)
        {
            inputs[ADSR_NOTE].val = 0;
        }

        // Set the current output sample to the current amplitude
        out[i] = current_amplitude;

        // Determine whether or not a note on value is detected
        bool note_on = inputs[ADSR_NOTE].val == 1;

        // Do something different based on the current envelope stage
        switch(adsr_stage)
        {
        // During the attack stage, note on will result in incrementing
        // towards full amplitude and switching to the decay stage when full
        // amplitude is reached, note off will result in skipping ahead to
        // the release stage
        case ADSR_A_STAGE:
            if(note_on)
            {
                double increment = 1 / ((inputs[ADSR_A].val / 1000)
                                        * SAMPLE_RATE);
                current_amplitude += increment;
                if(current_amplitude >= 1)
                {
                    current_amplitude = 1;
                    adsr_stage = ADSR_D_STAGE;
                }
            }
            else
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the decay stage, note on will result in decrementing
        // towards sustain amplitude and switching to the sustain stage once
        // sustain amplitude is reached, note off will result in skipping
        // ahead to the release stage
        case ADSR_D_STAGE:
            if(note_on)
            {
                double decrement = (1 - inputs[ADSR_S].val)
                                   / ((inputs[ADSR_D].val / 1000)
                                      * SAMPLE_RATE);
                current_amplitude -= decrement;
                if(current_amplitude <= inputs[ADSR_S].val)
                {
                    current_amplitude = inputs[ADSR_S].val;
                    adsr_stage = ADSR_S_STAGE;
                }
            }
            else
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the sustain stage, note on does nothing, note off will
        // result in skipping ahead to the release stage
        case ADSR_S_STAGE:
            if(!note_on)
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the release stage, note on will result in switching back
        // to the attack stage, note off will result in decrementing towards
        // 0 amplitude, and switching to the idle stage once 0 amplitude is
        // reached
        case ADSR_R_STAGE:
            if(note_on)
            {
                adsr_stage = ADSR_A_STAGE;
            }
            else
            {
                double decrement = inputs[ADSR_S].val
                                   / ((inputs[ADSR_R].val / 1000)
                                      * SAMPLE_RATE);
                current_amplitude -= decrement;
                if(current_amplitude <= 0)
                {
                    adsr_stage = ADSR_IDLE_STAGE;
                    current_amplitude = 0;
                }
            }
            break;
        // During the idle stage, note on will result in switching to the
        // attack stage, note off does nothing
        case ADSR_IDLE_STAGE:
            if(note_on)
            {
                adsr_stage = ADSR_A_STAGE;
            }
            break;
        }
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Adsr::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle reset stage button
    else if(g == graphics_objects["reset stage button"])
    {
        reset_stage();
        return true;
    }
    // If none of the above, handle events that apply to all modules, return
    // true if an event is handled
    else if(Module::handle_event(g))
    {
        return true;
    }

    // If none of the above, return false
    return false;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Adsr::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Reset stage button location
    location = {upper_left.x + MODULE_WIDTH - 15, upper_left.y, 7, 9};
    graphics_object_locations["reset stage button"] = location;

    // Waveform viewer location
    location = {upper_left.x, location.y + 15, MODULE_WIDTH, 54};
    graphics_object_locations["waveform"] = location;

    // Note on/off related graphics object locations
    location = {upper_left.x + 2, location.y + 57, 0, 0};
    graphics_object_locations["note text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["note text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["note toggle button"] = location;

    // Attack/decay related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["attack/decay text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["attack text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["attack toggle button"] = location;
    location = {location.x + location.w + 1,
                location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["decay text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["decay toggle button"] = location;

    // Sustain/release related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["sustain/release text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["sustain text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["sustain toggle button"] = location;
    location = {location.x + location.w + 1,
                location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["release text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["release toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Adsr::initialize_unique_graphics_objects()
{
    // Initialize reset stage button
    graphics_objects["reset stage button"] =
        new Button(name + " reset stage button",
                   graphics_object_locations["reset stage button"],
                   secondary_module_color, primary_module_color, "0",
                   this);

    // Initialize text objects
    graphics_objects["note text"] =
        new Text(name + " note text",
                 graphics_object_locations["note text"],
                 secondary_module_color, "NOTE ON/OFF:");
    graphics_objects["attack/decay text"] =
        new Text(name + " attack/decay text",
                 graphics_object_locations["attack/decay text"],
                 secondary_module_color, "ATTACK & DECAY:");
    graphics_objects["sustain/release text"] =
        new Text(name + " sustain/release text",
                 graphics_object_locations["sustain/release text"],
                 secondary_module_color, "SUSTAIN & RELEASE:");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform", graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text box objects
    graphics_objects["note text box"] =
        new Text_Box(name + " note text box",
                     graphics_object_locations["note text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["attack text box"] =
        new Text_Box(name + " attack text box",
                     graphics_object_locations["attack text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["decay text box"] =
        new Text_Box(name + " decay text box",
                     graphics_object_locations["decay text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["sustain text box"] =
        new Text_Box(name + " sustain text box",
                     graphics_object_locations["sustain text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["release text box"] =
        new Text_Box(name + " release text box",
                     graphics_object_locations["release text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);

    // Initialize toggle button objects
    graphics_objects["note toggle button"] =
        new Toggle_Button(name + " note toggle button",
                          graphics_object_locations["note toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["attack toggle button"] =
        new Toggle_Button(name + " attack toggle button",
                          graphics_object_locations["attack toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["decay toggle button"] =
        new Toggle_Button(name + " decay toggle button",
                          graphics_object_locations["decay toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["sustain toggle button"] =
        new Toggle_Button(name + " sustain toggle button",
                          graphics_object_locations["sustain toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["release toggle button"] =
        new Toggle_Button(name + " release toggle button",
                          graphics_object_locations["release toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["note text box"]] = ADSR_NOTE;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["note toggle button"]] = ADSR_NOTE;
    inputs[ADSR_NOTE].text_box = (Text_Box *) graphics_objects["note text box"];
    inputs[ADSR_NOTE].toggle_button = (Toggle_Button *) graphics_objects["note toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["attack text box"]] = ADSR_A;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["attack toggle button"]] = ADSR_A;
    inputs[ADSR_A].text_box = (Text_Box *) graphics_objects["attack text box"];
    inputs[ADSR_A].toggle_button = (Toggle_Button *) graphics_objects["attack toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["decay text box"]] = ADSR_D;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["decay toggle button"]] = ADSR_D;
    inputs[ADSR_D].text_box = (Text_Box *) graphics_objects["decay text box"];
    inputs[ADSR_D].toggle_button = (Toggle_Button *) graphics_objects["decay toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["sustain text box"]] = ADSR_S;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["sustain toggle button"]] = ADSR_S;
    inputs[ADSR_S].text_box = (Text_Box *) graphics_objects["sustain text box"];
    inputs[ADSR_S].toggle_button = (Toggle_Button *) graphics_objects["sustain toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["release text box"]] = ADSR_R;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["release toggle button"]] = ADSR_R;
    inputs[ADSR_R].text_box = (Text_Box *) graphics_objects["release text box"];
    inputs[ADSR_R].toggle_button = (Toggle_Button *) graphics_objects["release toggle button"];
}

std::string Adsr::get_unique_text_representation()
{
    return std::to_string(current_amplitude) + "\n"
           + std::to_string(adsr_stage) + "\n";
}

/*
 * Reset this ADSR's amplitude
 */
void Adsr::reset_stage()
{
    current_amplitude = 0;
    adsr_stage = ADSR_A_STAGE;

    std::cout << name << " stage reset" << std::endl;
}

