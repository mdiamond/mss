/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
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
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Output.hpp"

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Output::Output() :
    Module(OUTPUT)
{
    name = "output";
}

/*
 * Destructor.
 */
Output::~Output()
{}

/*
 * This function simply calls upon dependencies for processing. The output
 * module depends on all other modules.
 */
void Output::process()
{
    process_dependencies();
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Output::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle audio toggle button
    else if(g == graphics_objects["audio toggle button"])
    {
        toggle_audio();
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
void Output::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer related graphics object locations
    location = {upper_left.x, upper_left.y + 15, (MODULE_WIDTH / 2) - 1, 54};
    graphics_object_locations["waveform left"] = location;
    location = {location.x + location.w + 1, location.y, MODULE_WIDTH / 2, 54};
    graphics_object_locations["waveform right"] = location;

    // Input left related graphics object locations
    location = {upper_left.x + 2, location.y + 57, 0, 0};
    graphics_object_locations["input left text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["input left text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["input left toggle button"] = location;

    // Input right related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["input right text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["input right text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["input right toggle button"] = location;

    // Audio on/off related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["audio text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH, 9};
    graphics_object_locations["audio toggle button"] = location;

    // Remove the remove module button location
    graphics_object_locations.erase("remove module button");
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Output::initialize_unique_graphics_objects()
{
    // Initialize text objects
    graphics_objects["input left text"] =
        new Text(name + " input left text",
                 graphics_object_locations["input left text"],
                 secondary_module_color, "INPUT LEFT:");
    graphics_objects["input right text"] =
        new Text(name + " input right text",
                 graphics_object_locations["input right text"],
                 secondary_module_color, "INPUT RIGHT:");
    graphics_objects["audio text"] =
        new Text(name + " audio text",
                 graphics_object_locations["audio text"],
                 secondary_module_color, "AUDIO:");

    // Initialize waveform viewers
    graphics_objects["waveform left"] =
        new Waveform(name + " waveform left",
                     graphics_object_locations["waveform left"],
                     primary_module_color, secondary_module_color, NULL);
    graphics_objects["waveform right"] =
        new Waveform(name + " waveform right",
                     graphics_object_locations["waveform right"],
                     primary_module_color, secondary_module_color, NULL);


    // Initialize text boxes
    graphics_objects["input left text box"] =
        new Text_Box(name + " input left text box",
                     graphics_object_locations["input left text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["input right text box"] =
        new Text_Box(name + " input right text box",
                     graphics_object_locations["input right text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);

    // Initialize toggle buttons
    graphics_objects["input left toggle button"] =
        new Toggle_Button(name + " input left toggle button",
                          graphics_object_locations["input left toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["input right toggle button"] =
        new Toggle_Button(name + " input right toggle button",
                          graphics_object_locations["input right toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["input left text box"]] = OUTPUT_INPUT_L;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["input left toggle button"]] = OUTPUT_INPUT_L;
    inputs[OUTPUT_INPUT_L].text_box = (Text_Box *) graphics_objects["input left text box"];
    inputs[OUTPUT_INPUT_L].toggle_button = (Toggle_Button *) graphics_objects["input left toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["input right text box"]] = OUTPUT_INPUT_R;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["input right toggle button"]] = OUTPUT_INPUT_R;
    inputs[OUTPUT_INPUT_R].text_box = (Text_Box *) graphics_objects["input right text box"];
    inputs[OUTPUT_INPUT_R].toggle_button = (Toggle_Button *) graphics_objects["input right toggle button"];

    // Initialize filter type toggle buttons
    if(COLORBLIND_ON)
    {
        graphics_objects["audio toggle button"] =
            new Toggle_Button(name + " audio toggle button",
                              graphics_object_locations["audio toggle button"],
                              BLACK, WHITE, WHITE, BLACK,
                              "ON", "OFF", true, (Graphics_Listener *) this);
    }
    else
    {
        graphics_objects["audio toggle button"] =
            new Toggle_Button(name + " audio toggle button",
                              graphics_object_locations["audio toggle button"],
                              GREEN, RED, BLACK, WHITE,
                              "ON", "OFF", true, (Graphics_Listener *) this);
    }

    // Remove the remove module button
    graphics_objects.erase("remove module button");
}

/*
 * Toggle audio processing by either starting or pausing SDL audio, which will
 * halt or start calling of the callback function.
 */
void Output::toggle_audio()
{
    AUDIO_ON = !AUDIO_ON;
    ((Toggle_Button *) graphics_objects["audio toggle button"])->toggle();

    if(AUDIO_ON)
    {
        SDL_PauseAudio(0);
    }
    else
    {
        SDL_PauseAudio(1);
    }

    std::cout << "Audio is now " << (AUDIO_ON ? "on" : "off") << std::endl;
}

std::string Output::get_unique_text_representation()
{
    return "";
}

