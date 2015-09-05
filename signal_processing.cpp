/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer.
 */

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"

using namespace std;

/*
 * The primary helper function for the audio callback function.
 * Figure out how many samples are being worked with, then generate
 * data to fill the buffer.
 */
void fill_buffer(Uint8 *_buffer, int length)
{
  // Float samples are 32 bits, 4 bytes * 2 (stereo)
  // is 8 bytes per unique sample, 4 bytes for left, 4 for right
  int num_unique_samples = length / 8;
  // 4 bytes per individual sample
  int num_samples = length / 4;
  // Cast the buffer pointer to a float pointer to
  // allow inserting floats into its memory locations
  float *buffer = (float *) _buffer;
  // Start at the first sample in the buffer
  int index = 0;
  while(num_unique_samples > 0)
  {
    float wave_fraction = (CURRENT_SAMPLE / (SAMPLE_RATE / (FREQUENCY * 2)));
    float sample_amplitude = sin(M_PI * wave_fraction);
    buffer[index] = sample_amplitude;
    buffer[index + 1] = sample_amplitude;
    index += 2;
    CURRENT_SAMPLE ++;
    AUDIO_LENGTH --;
    num_unique_samples --;
  }
}
