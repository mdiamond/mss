/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer.
 */

// Included libraries
#include <iostream>
#include <vector>

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

/*
 * Open the audio device with the specified configuration
 */
int open_audio_device(void)
{
  SDL_AudioSpec wanted, obtained;
  
  wanted.freq = SAMPLE_RATE;
  wanted.format = AUDIO_F32SYS;
  wanted.channels = 2;
  wanted.samples = 512;
  wanted.callback = audio_callback;
  wanted.userdata = NULL;

  if(SDL_OpenAudio(&wanted, &obtained) == -1)
    return 0;

  cout << "Audio details:" << endl;
  cout << "Sample rate: " << obtained.freq << endl;
  cout << "Format: " << obtained.format << endl;
  cout << "Channels: " << obtained.channels << endl;
  cout << "Buffer size: " << obtained.samples << endl;

  BUFFER_SIZE = obtained.samples;

  // Return success
  return 1;
}

/*
 * Add two signals
 */
void add_signals(vector<float> *buffer1, vector<float> *buffer2, vector<float> *result_buffer, int length)
{
  // 4 bytes per float
  int num_samples = length / 4;

  // For each sample
  for(int i = 0; i < num_samples; i ++)
  {
    // Sum the samples from the two input buffers into the result buffer
    (*result_buffer)[i] = (*buffer1)[i] + (*buffer2)[i];
  }
}
