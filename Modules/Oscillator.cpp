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
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "Oscillator.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name, int _number)
{
    name = *_name;
    type = OSCILLATOR;
    number = _number;

    audio.frequency = 0;
    audio.frequency_str = "0";
    audio.input_frequency = new vector<float>(BUFFER_SIZE, 0);
    audio.live_frequency = false;

    audio.phase_offset = 0;
    audio.phase_offset_str = "0";
    audio.input_phase_offset = new vector<float>(BUFFER_SIZE, 0);
    audio.live_phase_offset = false;

    audio.pulse_width = 0;
    audio.pulse_width_str = "0";
    audio.input_pulse_width = new vector<float>(BUFFER_SIZE, 0);
    audio.live_pulse_width = false;

    audio.range_low = -1;
    audio.range_low_str = "-1";
    audio.input_range_low = new vector<float>(BUFFER_SIZE, 0);
    audio.live_range_low = false;

    audio.range_high = 1;
    audio.range_high_str = "1";
    audio.input_range_high = new vector<float>(BUFFER_SIZE, 0);
    audio.live_range_high = false;

    output = new vector<float>(BUFFER_SIZE, 0);
    audio.output = new vector<float>(BUFFER_SIZE, 0);
    graphics.output = new vector<float>(BUFFER_SIZE, 0);

    audio.waveform_type = SIN;
}

/*
 * Dummy function
 */
Oscillator::~Oscillator()
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Oscillator::process()
{
    // Check for any dependencies for frequency modulation
    process_depends();

    if(audio.live_frequency)
        audio.frequency = (*(audio.input_frequency))[0];
    if(audio.live_phase_offset)
        audio.phase_offset = (*(audio.input_phase_offset))[0];
    if(audio.live_pulse_width)
        audio.pulse_width = (*(audio.input_pulse_width))[0];
    if(audio.live_range_low)
        audio.range_low = (*(audio.input_range_low))[0];
    if(audio.live_range_high)
        audio.range_high = (*(audio.input_range_high))[0];

    // Calculate an amplitude for each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Calculate and store the current samples amplitude
        // based on phase
        (*(output))[i] = sin(audio.current_phase);
        audio.current_phase += (2 * M_PI * audio.frequency / SAMPLE_RATE);
        if(audio.current_phase > (2 * M_PI))
            audio.current_phase -= (2 * M_PI);
    }
    if(audio.range_low != -1 || audio.range_high != 1)
    {
        scale_signal(output, -1, 1, audio.range_low, audio.range_high);
    }
}

/*
 * Calculate the location of the waveform visualizer within
 * the window, then create a waveform object at that location
 * to make use of this oscillator's output buffer for its
 * audio data.
 */
Graphics_Object *Oscillator::calculate_waveform_visualizer()
{
    SDL_Rect location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                          upper_left.y + MODULE_BORDER_WIDTH + 23,
                          ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4),
                          55};
    string object_name = "waveform visualizer (waveform)";
    Waveform *graphics_object = new Waveform(&object_name, &location, &WHITE, output);
    return graphics_object;
}

Graphics_Object *Oscillator::calculate_range_high()
{
    int x = upper_left.x + (MODULE_WIDTH / 2) + 1;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 204;
    SDL_Rect location = {x, y, (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3), 15};
    string object_name = "oscillator range high (text_box)";
    string prompt = "";
    string text = "";
    Text_Box *graphics_object = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.range_high_str),
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return graphics_object;
}

Graphics_Object *Oscillator::calculate_range_low()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 204;
    SDL_Rect location = {x, y, (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3), 15};
    string object_name = "oscillator range low (text_box)";
    string prompt = "";
    string text = "";
    Text_Box *graphics_object = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.range_low_str),
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return graphics_object;
}

Graphics_Object *Oscillator::calculate_pulse_width()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 169;
    SDL_Rect location = {x, y, ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4), 15};
    string object_name = "oscillator pulse width (text_box)";
    string prompt = "float or module name";
    string text = "";
    Text_Box *graphics_object = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.pulse_width_str),
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return graphics_object;
}

Graphics_Object *Oscillator::calculate_phase_offset()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 134;
    SDL_Rect location = {x, y, ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4), 15};
    string object_name = "oscillator phase offset (text_box)";
    string prompt = "float or module name";
    string text = "";
    Text_Box *graphics_object = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.phase_offset_str),
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return graphics_object;
}

/*
 * Calculate the location of the text box for
 * frequency of the oscillator.
 */
Graphics_Object *Oscillator::calculate_frequency()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 97;
    SDL_Rect location = {x, y, ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4), 15};
    string object_name = "oscillator frequency (text_box)";
    string prompt = "float or module name";
    string text = "";
    Text_Box *graphics_object = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.frequency_str),
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return graphics_object;
}

void Oscillator::calculate_text_objects()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 80;
    SDL_Rect location = {x, y, 0, 0};
    string object_name = "oscillator frequency (text)";
    string contents = "FREQUENCY: ";
    Text *text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
    graphics_objects.push_back(text);

    x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    y = upper_left.y + MODULE_BORDER_WIDTH + 117;
    location = {x, y, 0, 0};
    object_name = "oscillator phase offset (text)";
    contents = "PHASE OFFSET: ";
    text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
    graphics_objects.push_back(text);

    x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    y = upper_left.y + MODULE_BORDER_WIDTH + 152;
    location = {x, y, 0, 0};
    object_name = "oscillator pulse width (text)";
    contents = "PULSE WIDTH: ";
    text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
    graphics_objects.push_back(text);

    x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    y = upper_left.y + MODULE_BORDER_WIDTH + 187;
    location = {x, y, 0, 0};
    object_name = "oscillator range high/low (text)";
    contents = "RANGE HIGH/LOW: ";
    text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
    graphics_objects.push_back(text);
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
    calculate_text_objects();
    graphics_objects.push_back(calculate_waveform_visualizer());
    graphics_objects.push_back(calculate_frequency());
    graphics_objects.push_back(calculate_phase_offset());
    graphics_objects.push_back(calculate_pulse_width());
    graphics_objects.push_back(calculate_range_low());
    graphics_objects.push_back(calculate_range_high());
}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Oscillator::copy_graphics_data()
{
    graphics.waveform_type = audio.waveform_type;

    graphics.frequency_str = to_string(audio.frequency);
    graphics.phase_offset_str = to_string(audio.phase_offset);
    graphics.pulse_width_str = to_string(audio.pulse_width);
    graphics.range_low_str = to_string(audio.range_low);
    graphics.range_high_str = to_string(audio.range_high);

    copy_signal(output, graphics.output);
}

void Oscillator::set_frequency(float _frequency)
{
    audio.frequency = _frequency;
}
