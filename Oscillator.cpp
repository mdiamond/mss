/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class.
 */

// Include header file
#include "Oscillator.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name)
{
  name = *_name;
  type = OSCILLATOR;

  color.r = 0;
  color.g = 255;
  color.b = 0;
  color.a = 255;

  this->audio.frequency = 0;
  this->audio.phase = 0;
  this->audio.amplitude = 1;
  this->audio.fm_on = 0;
  this->audio.modulation_index = 1;
  this->audio.output = new vector<float>(BUFFER_SIZE, 0);

  modulator = NULL;
}

/*
 * Dummy function
 */
Oscillator::~Oscillator(void)
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
      audio_data->phase += (2 * M_PI * (audio_data->frequency + frequency_shift) / SAMPLE_RATE);
    }
    else
      audio_data->phase += (2 * M_PI * audio_data->frequency / SAMPLE_RATE);
    if(audio_data->phase > (2 * M_PI))
      audio_data->phase -= (2 * M_PI);
  }
}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Oscillator::copy_graphics_data()
{
  this->graphics.frequency = this->audio.frequency;
  this->graphics.phase = this->audio.phase;
  this->graphics.amplitude = this->audio.amplitude;
  this->graphics.fm_on = this->audio.fm_on;
  this->graphics.modulation_index = this->audio.modulation_index;
  this->graphics.output = new vector<float>(*(this->audio.output));
}

/*
 * This function uses the renderer to create a representation
 * of this module in the window.
 */
void Oscillator::render()
{

}
