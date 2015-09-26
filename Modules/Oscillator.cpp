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
#include <sstream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../image_processing.hpp"
#include "../main.hpp"

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
    audio.shifted_frequency = 0;
    audio.frequency_str = "0";
    audio.phase = 0;
    audio.amplitude = 1;
    audio.fm_on = 0;
    audio.modulation_index = 1;
    audio.output = new vector<float>(BUFFER_SIZE, 0);

    graphics.frequency_str = "0";
    graphics.output = new vector<float>(BUFFER_SIZE, 0);

    modulator = NULL;
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
    float frequency_shift = 0;

    // Check for any dependencies for frequency modulation
    process_depends();

    // audio.frequency = stof(((Text_Box *) graphics_objects[4])->typing_buffer);
    audio.shifted_frequency = audio.frequency;
    // Calculate an amplitude for each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Calculate and store the current samples amplitude
        // based on phase
        (*(audio.output))[i] = audio.amplitude * sin(audio.phase);
        // Calculate phase for the next sample
        if(audio.fm_on)
        {
            frequency_shift = (((modulator->audio).frequency) * audio.modulation_index) * (*((modulator->audio).output))[i];
            audio.shifted_frequency = audio.frequency + frequency_shift;
            audio.phase += (2 * M_PI * (audio.shifted_frequency) / SAMPLE_RATE);
        }
        else
            audio.phase += (2 * M_PI * audio.frequency / SAMPLE_RATE);
        if(audio.phase > (2 * M_PI))
            audio.phase -= (2 * M_PI);
    }
}

/*
 * Calculate the location of the waveform visualizer within
 * the window, then create a waveform object at that location
 * to make use of this oscillator's output buffer for its
 * audio data.
 */
Graphics_Object *Oscillator::calculate_waveform()
{
    SDL_Rect location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                          upper_left.y + MODULE_BORDER_WIDTH + 18,
                          ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4),
                          50};
    string object_name = "waveform";
    Waveform *waveform = new Waveform(&object_name, &location, &WHITE, graphics.output);
    return waveform;
}

/*
 * Calculate the location of the frequency of the oscillator.
 */
Graphics_Object *Oscillator::calculate_frequency()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 90;
    SDL_Rect location = {x, y, 0, 0};
    string object_name = "oscillator frequency";
    Text_Box *frequency = new Text_Box(&object_name, &location, &text_color,
                                       &(graphics.frequency_str),
                                       &(graphics.frequency_str),
                                       FONT_REGULAR);
    return frequency;
}

void Oscillator::calculate_text_objects()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 70;
    SDL_Rect location = {x, y, 0, 0};
    string object_name = "oscillator frequency";
    string contents = "FREQUENCY: ";
    Text *frequency = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
    graphics_objects.push_back(frequency);
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
    calculate_text_objects();
    graphics_objects.push_back(calculate_waveform());
    graphics_objects.push_back(calculate_frequency());
}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Oscillator::copy_graphics_data()
{
    ostringstream ss;
    graphics.frequency = audio.frequency;
    graphics.shifted_frequency = audio.shifted_frequency;
    ss << graphics.shifted_frequency;
    string frequency_str(ss.str());
    graphics.frequency_str = frequency_str;
    graphics.phase = audio.phase;
    graphics.amplitude = audio.amplitude;
    graphics.fm_on = audio.fm_on;
    graphics.modulation_index = audio.modulation_index;
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        (*(graphics.output))[i] = (*(audio.output))[i];
    }
}

void Oscillator::set_frequency(float _frequency)
{
    audio.frequency = _frequency;
}
