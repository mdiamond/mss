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
#include "function_forwarder.hpp"
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Output.hpp"

// Included graphics classes
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

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
    // if g is null, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle events that apply to all modules, return true if an event
    // is handled
    else if(Module::handle_event(g))
    {
        return true;
    }
    else if(g == graphics_objects["audio on/off toggle button"])
    {
        toggle_audio();
        return true;
    }

    // If none of the above happen, return false
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
    location = {upper_left.x, location.y + 10, 0, 0};
    graphics_object_locations["audio on/off text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH, 9};
    graphics_object_locations["audio on/off toggle button"] = location;

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
    graphics_objects["audio on/off text"] =
        new Text(name + " audio on/off text",
                 graphics_object_locations["audio on/off text"],
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


    // Initialize input text boxes
    graphics_objects["input left text box"] =
        new Input_Text_Box(name + " input left text box",
                           graphics_object_locations["input left text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, OUTPUT_INPUT_L, NULL);
    graphics_objects["input right text box"] =
        new Input_Text_Box(name + " input right text box",
                           graphics_object_locations["input right text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, OUTPUT_INPUT_R, NULL);

    // Initialize input toggle buttons
    graphics_objects["input left toggle button"] =
        new Input_Toggle_Button(name + " input left toggle button",
                                graphics_object_locations["input left toggle button"],
                                secondary_module_color, primary_module_color,
                                this, OUTPUT_INPUT_L,
                                (Input_Text_Box *) graphics_objects["input left text box"]);
    graphics_objects["input right toggle button"] =
        new Input_Toggle_Button(name + " input right toggle button",
                                graphics_object_locations["input right toggle button"],
                                secondary_module_color, primary_module_color,
                                this, OUTPUT_INPUT_R,
                                (Input_Text_Box *) graphics_objects["input right text box"]);

    // Point input text boxes to their associated input toggle buttons
    ((Input_Text_Box *)
     graphics_objects["input left text box"])->input_toggle_button =
        (Input_Toggle_Button *) graphics_objects["input left toggle button"];
    ((Input_Text_Box *)
     graphics_objects["input right text box"])->input_toggle_button =
        (Input_Toggle_Button *) graphics_objects["input right toggle button"];

    // Initialize filter type toggle buttons
    if(COLORBLIND_ON)
    {
        graphics_objects["audio on/off toggle button"] =
            new Toggle_Button(name + " audio on/off toggle button",
                              graphics_object_locations["audio on/off toggle button"],
                              BLACK, WHITE,
                              WHITE, BLACK,
                              FONT, "ON", "OFF", true, this);
    }
    else
    {
        graphics_objects["audio on/off toggle button"] =
            new Toggle_Button(name + " audio on/off toggle button",
                              graphics_object_locations["audio on/off toggle button"],
                              GREEN, RED,
                              BLACK, WHITE,
                              FONT, "ON", "OFF", true, this);
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

    if(AUDIO_ON)
    {
        SDL_PauseAudio(0);
    }
    else
    {
        SDL_PauseAudio(1);
    }

    std::cout << "Audio toggled" << std::endl;
}

std::string Output::get_unique_text_representation()
{
    return "";
}

