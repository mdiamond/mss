/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class.
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
#include "Modules/Oscillator.hpp"

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor.
 */
Oscillator::Oscillator() :
    Module(OSCILLATOR),
    phase(0), previous_phase_offset(0),
    waveform_type(SIN), sin_on(true), tri_on(false),
    saw_on(false), sqr_on(false)
{
    inputs[OSCILLATOR_PULSE_WIDTH].val = .5;
    inputs[OSCILLATOR_RANGE_LOW].val = -1;
    inputs[OSCILLATOR_RANGE_HIGH].val = 1;
}

/*
 * Destructor.
 */
Oscillator::~Oscillator()
{}

/*
 * Given a phase from 0 to 1, calculate and return a sine wave sample.
 */
double Oscillator::produce_sin_sample(double phase)
{
    return sin(2 * M_PI * phase);
}

/*
 * Given a phase from 0 to 1, calculate and return a triangle wave sample.
 */
double Oscillator::produce_tri_sample(double phase)
{
    if(phase < .25)
    {
        return phase / .25;
    }
    else if(phase < .5)
    {
        return 1 - ((phase - .25) / .25);
    }
    else if(phase < .75)
    {
        return 0 - ((phase - .5) / .25);
    }
    else
    {
        return ((phase - .75) / .25) - 1;
    }
}

/*
 * Given a phase from 0 to 1, calculate and return a saw wave sample.
 */
double Oscillator::produce_saw_sample(double phase)
{
    if(phase < .5)
    {
        return -1 * (1 - (phase / .5));
    }
    else
    {
        return -1 * (0 - ((phase - .5) / .5));
    }
}

/*
 * Given a phase from 0 to 1, calculate and return a square wave sample.
 */
double Oscillator::produce_sqr_sample(double phase)
{
    if(phase < inputs[OSCILLATOR_PULSE_WIDTH].val)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

/*
 * Process all dependencies, then fill the output buffer with a waveform given
 * the data contained within this class and the audio device information.
 */
void Oscillator::process()
{
    // Process any dependencies
    process_dependencies();

    double phase_offset_diff;
    // Calculate an amplitude for each sample
    for(unsigned short i = 0; i < out.size(); i ++)
    {
        update_input_vals(i);

        phase_offset_diff = inputs[OSCILLATOR_PHASE_OFFSET].val
                            - previous_phase_offset;
        previous_phase_offset = inputs[OSCILLATOR_PHASE_OFFSET].val;

        // Calculate and store the current samples amplitude
        // based on phase
        if(inputs[OSCILLATOR_FREQUENCY].val < 1
           && inputs[OSCILLATOR_FREQUENCY].val > -1)
        {
            switch(waveform_type)
            {
            case SIN :
                out[i] = produce_sin_sample(phase);
                break;
            case TRI :
                out[i] = produce_tri_sample(phase);
                break;
            case SAW:
                out[i] = produce_saw_sample(phase);
                break;
            case SQR:
                out[i] = produce_sqr_sample(phase);
                break;
            }
        }
        else if(waveform_type != SQR || inputs[OSCILLATOR_PULSE_WIDTH].val == .5)
            out[i] = WAVETABLES[waveform_type][(int)(phase
                                                        * SAMPLE_RATE)];
        else
        {
            out[i] = produce_sqr_sample(phase);
        }

        // If the oscillator has an abnormal range, scale the sample to
        // that range
        if(inputs[OSCILLATOR_RANGE_LOW].val != -1
           || inputs[OSCILLATOR_RANGE_HIGH].val != 1)
            out[i] = scale_sample(out[i], -1, 1,
                                     inputs[OSCILLATOR_RANGE_LOW].val,
                                     inputs[OSCILLATOR_RANGE_HIGH].val);

        // Increment the current phase according to the frequency, sample rate,
        // and difference in phase offset since the last sample was calculated
        phase += ((double) inputs[OSCILLATOR_FREQUENCY].val
                          / SAMPLE_RATE) + phase_offset_diff;
        // Wrap around if the phase goes above 1 or below 0
        while(phase > 1)
        {
            phase -= 1;
        }
        while(phase < 0)
        {
            phase += 1;
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
bool Oscillator::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle reset phase button
    else if(g == graphics_objects["reset phase button"])
    {
        reset_phase();
        return true;
    }
    // Handle sin toggle button
    else if(g == graphics_objects["sin toggle button"])
    {
        switch_waveform(SIN);
        return true;
    }
    // Handle tri toggle button
    else if(g == graphics_objects["tri toggle button"])
    {
        switch_waveform(TRI);
        return true;
    }
    // Handle saw toggle button
    else if(g == graphics_objects["saw toggle button"])
    {
        switch_waveform(SAW);
        return true;
    }
    // Handle sqr toggle button
    else if(g == graphics_objects["sqr toggle button"])
    {
        switch_waveform(SQR);
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
void Oscillator::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Reset buffer button location
    location = {upper_left.x + MODULE_WIDTH - 15, upper_left.y, 7, 9};
    graphics_object_locations["reset phase button"] = location;

    // Waveform viewer location
    location = {upper_left.x, location.y + 15, MODULE_WIDTH, 34};
    graphics_object_locations["waveform"] = location;

    // Frequency related graphics object locations
    location = {upper_left.x + 2, location.y + 37, 0, 0};
    graphics_object_locations["frequency text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["frequency text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["frequency toggle button"] = location;

    // Phase offset/pulse width related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["phase offset/pulse width text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["phase offset text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["phase offset toggle button"] = location;
    location = {location.x + location.w + 1, location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["pulse width text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["pulse width toggle button"] = location;

    // Range low/range high related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["ranges text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["range low text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["range low toggle button"] = location;
    location = {location.x + location.w + 1, location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["range high text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["range high toggle button"] = location;

    // Waveform type related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["waveform type text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 4) - 1, 9};
    graphics_object_locations["sin toggle button"] = location;
    location = {location.x + location.w + 1, location.y, location.w, 9};
    graphics_object_locations["tri toggle button"] = location;
    location = {location.x + location.w + 1, location.y, location.w, 9};
    graphics_object_locations["saw toggle button"] = location;
    location = {location.x + location.w + 1, location.y, location.w, 9};
    graphics_object_locations["sqr toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Oscillator::initialize_unique_graphics_objects()
{
    // Initialize reset buffer button
    graphics_objects["reset phase button"] =
        new Button(name + " reset phase button",
                   graphics_object_locations["reset phase button"],
                   secondary_module_color, primary_module_color, "0", this);

    // Initialize text objects
    graphics_objects["frequency text"] =
        new Text(name + " frequency text",
                 graphics_object_locations["frequency text"],
                 secondary_module_color, "FREQUENCY (Hz):");
    graphics_objects["phase offset/pulse width text"] =
        new Text(name + " phase offset text",
                 graphics_object_locations["phase offset text"],
                 secondary_module_color, "OFFSET & WIDTH (0-1):");
    graphics_objects["ranges text"] =
        new Text(name + " ranges text",
                 graphics_object_locations["ranges text"],
                 secondary_module_color, "RANGE LOW & HIGH (#):");
    graphics_objects["waveform type text"] =
        new Text(name + " waveform type text",
                 graphics_object_locations["waveform type text"],
                 secondary_module_color, "WAVEFORM TYPE:");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text boxes
    graphics_objects["frequency text box"] =
        new Text_Box(name + " frequency text box",
                     graphics_object_locations["frequency text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["phase offset text box"] =
        new Text_Box(name + " phase offset text box",
                     graphics_object_locations["phase offset text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["pulse width text box"] =
        new Text_Box(name + " pulse width text box",
                     graphics_object_locations["pulse width text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["range low text box"] =
        new Text_Box(name + " range low text box",
                     graphics_object_locations["range low text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["range high text box"] =
        new Text_Box(name + " range high text box",
                     graphics_object_locations["range high text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);

    // Initialize toggle buttons
    graphics_objects["frequency toggle button"] =
        new Toggle_Button(name + " frequency toggle button",
                          graphics_object_locations["frequency toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["phase offset toggle button"] =
        new Toggle_Button(name + " phase offset toggle button",
                          graphics_object_locations["phase offset toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["pulse width toggle button"] =
        new Toggle_Button(name + " pulse width toggle button",
                          graphics_object_locations["pulse width toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["range low toggle button"] =
        new Toggle_Button(name + " range low toggle button",
                          graphics_object_locations["range low toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["range high toggle button"] =
        new Toggle_Button(name + " range high toggle button",
                          graphics_object_locations["range high toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["frequency text box"]] = OSCILLATOR_FREQUENCY;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["frequency toggle button"]] = OSCILLATOR_FREQUENCY;
    inputs[OSCILLATOR_FREQUENCY].text_box = (Text_Box *) graphics_objects["frequency text box"];
    inputs[OSCILLATOR_FREQUENCY].toggle_button = (Toggle_Button *) graphics_objects["frequency toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["phase offset text box"]] = OSCILLATOR_PHASE_OFFSET;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["phase offset toggle button"]] = OSCILLATOR_PHASE_OFFSET;
    inputs[OSCILLATOR_PHASE_OFFSET].text_box = (Text_Box *) graphics_objects["phase offset text box"];
    inputs[OSCILLATOR_PHASE_OFFSET].toggle_button = (Toggle_Button *) graphics_objects["phase offset toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["pulse width text box"]] = OSCILLATOR_PULSE_WIDTH;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["pulse width toggle button"]] = OSCILLATOR_PULSE_WIDTH;
    inputs[OSCILLATOR_PULSE_WIDTH].text_box = (Text_Box *) graphics_objects["pulse width text box"];
    inputs[OSCILLATOR_PULSE_WIDTH].toggle_button = (Toggle_Button *) graphics_objects["pulse width toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["range low text box"]] = OSCILLATOR_RANGE_LOW;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["range low toggle button"]] = OSCILLATOR_RANGE_LOW;
    inputs[OSCILLATOR_RANGE_LOW].text_box = (Text_Box *) graphics_objects["range low text box"];
    inputs[OSCILLATOR_RANGE_LOW].toggle_button = (Toggle_Button *) graphics_objects["range low toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["range high text box"]] = OSCILLATOR_RANGE_HIGH;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["range high toggle button"]] = OSCILLATOR_RANGE_HIGH;
    inputs[OSCILLATOR_RANGE_HIGH].text_box = (Text_Box *) graphics_objects["range high text box"];
    inputs[OSCILLATOR_RANGE_HIGH].toggle_button = (Toggle_Button *) graphics_objects["range high toggle button"];

    // Initialize filter type toggle buttons
    graphics_objects["sin toggle button"] =
        new Toggle_Button(name + " sin toggle button",
                          graphics_object_locations["sin toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          "SIN", "SIN", sin_on, (Graphics_Listener *) this);
    graphics_objects["tri toggle button"] =
        new Toggle_Button(name + " tri toggle button",
                          graphics_object_locations["tri toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          "TRI", "TRI", tri_on, (Graphics_Listener *) this);
    graphics_objects["saw toggle button"] =
        new Toggle_Button(name + " saw toggle button",
                          graphics_object_locations["saw toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          "SAW", "SAW", saw_on, (Graphics_Listener *) this);
    graphics_objects["sqr toggle button"] =
        new Toggle_Button(name + " sqr toggle button",
                          graphics_object_locations["sqr toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          "SQR", "SQR", sqr_on, (Graphics_Listener *) this);
}

/*
 * Switch to outputting the given waveform type.
 */
void Oscillator::switch_waveform(WaveformType waveform_type_)
{
    sin_on = false;
    tri_on = false;
    saw_on = false;
    sqr_on = false;
    ((Toggle_Button *) graphics_objects["sin toggle button"])->b =
        false;
    ((Toggle_Button *) graphics_objects["tri toggle button"])->b =
        false;
    ((Toggle_Button *) graphics_objects["saw toggle button"])->b =
        false;
    ((Toggle_Button *) graphics_objects["sqr toggle button"])->b =
        false;

    switch(waveform_type_)
    {
    case SIN:
        waveform_type = SIN;
        sin_on = true;
        ((Toggle_Button *) graphics_objects["sin toggle button"])->b =
            true;
        std::cout << name << " is now outputting a sine wave" << std::endl;
        break;
    case TRI:
        waveform_type = TRI;
        tri_on = true;
        ((Toggle_Button *) graphics_objects["tri toggle button"])->b =
            true;
        std::cout << name << " is now outputting a triangle wave" << std::endl;
        break;
    case SAW:
        waveform_type = SAW;
        saw_on = true;
        ((Toggle_Button *) graphics_objects["saw toggle button"])->b =
            true;
        std::cout << name << " is now outputting a sawtooth wave" << std::endl;
        break;
    case SQR:
        waveform_type = SQR;
        sqr_on = true;
        ((Toggle_Button *) graphics_objects["sqr toggle button"])->b =
            true;
        std::cout << name << " is now outputting a square wave" << std::endl;
        break;
    }

    waveform_type = waveform_type_;
}

/*
 * Reset this oscillator's phase
 */
void Oscillator::reset_phase()
{
    phase = 0;

    std::cout << name << " phase reset" << std::endl;
}

std::string Oscillator::get_unique_text_representation()
{
    return std::to_string(phase) + "\n"
           + std::to_string(waveform_type) + "\n";
}

