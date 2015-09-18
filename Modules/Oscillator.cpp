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

    this->audio.frequency = 0;
    this->audio.shifted_frequency = 0;
    this->audio.frequency_str = "0";
    this->audio.phase = 0;
    this->audio.amplitude = 1;
    this->audio.fm_on = 0;
    this->audio.modulation_index = 1;
    this->audio.output = new vector<float>(BUFFER_SIZE, 0);

    this->graphics.frequency_str = "0";
    this->graphics.output = new vector<float>(BUFFER_SIZE, 0);

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

    // Get oscillator audio info struct
    struct Oscillator_Data *audio_data = &(this->audio);
    // Calculate an amplitude for each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Calculate and store the current samples amplitude
        // based on phase
        (*(audio_data->output))[i] = audio_data->amplitude * sin(audio_data->phase);
        // Calculate phase for the next sample
        if(audio_data->fm_on)
        {
            frequency_shift = (((modulator->audio).frequency) * audio_data->modulation_index) * (*((modulator->audio).output))[i];
            audio_data->shifted_frequency = audio_data->frequency + frequency_shift;
            audio_data->phase += (2 * M_PI * (audio_data->shifted_frequency) / SAMPLE_RATE);
        }
        else
            audio_data->phase += (2 * M_PI * audio_data->frequency / SAMPLE_RATE);
        if(audio_data->phase > (2 * M_PI))
            audio_data->phase -= (2 * M_PI);
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
    SDL_Rect _waveform = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                          upper_left.y + MODULE_BORDER_WIDTH + 18,
                          ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4),
                          50};
    string object_name = "waveform";
    Waveform *waveform = new Waveform(&object_name, &_waveform, &WHITE, this->graphics.output);
    return waveform;
}

/*
 * Calculate the location of the frequency of the oscillator.
 */
Graphics_Object *Oscillator::calculate_frequency()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 70;
    SDL_Rect location = {x, y, 0, 0};
    string object_name = "oscillator frequency";
    Text *frequency = new Text(&object_name, &location, &text_color, &(this->graphics.frequency_str), FONT_REGULAR);
    return frequency;
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
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
    this->graphics.frequency = this->audio.frequency;
    this->graphics.shifted_frequency = this->audio.shifted_frequency;
    ss << this->graphics.shifted_frequency;
    string frequency_str(ss.str());
    this->graphics.frequency_str = frequency_str;
    this->graphics.phase = this->audio.phase;
    this->graphics.amplitude = this->audio.amplitude;
    this->graphics.fm_on = this->audio.fm_on;
    this->graphics.modulation_index = this->audio.modulation_index;
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        (*(this->graphics.output))[i] = (*(this->audio.output))[i];
    }
}
