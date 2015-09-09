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

// Included classes
#include "Module.hpp"
#include "Oscillator.hpp"
#include "Output.hpp"

using namespace std;

/*
 * Audio callback which triggers the generation of samples
 * when more audio is needed to play. This function calls upon the
 * output module to process, which recursively triggers the processing
 * of samples down the entire signal chain. Once all samples are
 * processed and ready, the buffer is filled with the waiting samples
 * in the output modules left and right channel output buffers.
 */
void audio_callback(void *userdata, Uint8 *_buffer, int length)
{
  // Cast the buffer to a float buffer
  float *buffer = (float *) _buffer;

  // If there is no audio to play, just return
  if(AUDIO_LENGTH == 0)
    return;

  // Get the address of the output module for later use
  Output *output = (Output *) modules[0];

  // Process audio for the output module
  // This will recursively call upon depended modules
  // for processed audio, meaning that modules at
  // the beginning of the signal chain will be processed first
  output->process();

  // // Uncomment this and comment out the buffer filling code
  // // below for some cool AM action
  // Oscillator *oscillator_1 = (Oscillator *) modules[1];
  // Oscillator *oscillator_2 = (Oscillator *) modules[2];
  // float *buffer_l = buffer;
  // float *buffer_r = buffer + 1;
  // for(int i = 0; i < BUFFER_SIZE; i ++)
  // {
  //   cout << (*(oscillator_1->output))[i] * (*(oscillator_2->output))[i] << ", ";
  //   *buffer_l = (*(oscillator_1->output))[i] * (*(oscillator_2->output))[i];
  //   *buffer_r = (*(oscillator_1->output))[i] * (*(oscillator_2->output))[i];
  //   buffer_l += 2;
  //   buffer_r += 2;
  //   oscillator_1->frequency += .001;
  //   oscillator_2->frequency += .0001;
  // }

  // Fetch the output module's latest processed audio
  // and insert it into the buffer
  float *buffer_l = buffer;
  float *buffer_r = buffer + 1;
  for(int i = 0; i < BUFFER_SIZE; i ++)
  {
    *buffer_l = (*(output->input_l))[i];
    *buffer_r = (*(output->input_r))[i];
    buffer_l += 2;
    buffer_r += 2;
  }

  // Increment the current sample by the number
  // of samples just processed
  AUDIO_LENGTH -= BUFFER_SIZE;
}

/*
 * Open the audio device with a simple configuration.
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
  {
    cout << "Could not open the audio device: " << SDL_GetError() << endl;
    return 0;
  }

  cout << "Audio details:" << endl;
  cout << "  Sample rate: " << obtained.freq << endl;
  cout << "  Format: " << obtained.format << endl;
  cout << "  Channels: " << obtained.channels << endl;
  cout << "  Buffer size: " << obtained.samples << endl;

  BUFFER_SIZE = obtained.samples;

  // Return success
  return 1;
}

void initialize_output()
{
  // Create the output module
  Output *output = new Output();
  modules.push_back(output);

  // Create two oscillator modules
  string oscillator_1_name = "oscillator 1";
  Oscillator *oscillator_1 = new Oscillator(&oscillator_1_name);
  modules.push_back(oscillator_1);
  // string oscillator_2_name = "oscillator_2";
  // Oscillator  *oscillator_2 = new Oscillator(&oscillator_2_name);
  // modules.push_back(oscillator_2);

  // Set the inputs and outputs
  output->depends.push_back(oscillator_1);
  // output->depends.push_back(oscillator_2);
  output->input_l = oscillator_1->output;
  output->input_r = oscillator_1->output;

  // Set the oscillator frequencies
  oscillator_1->frequency = 440;
  // oscillator_2->frequency = 1;

  // Set up the FM oscillator
  // string oscillator_3_name = "oscillator 3";
  // Oscillator *oscillator_3 = new Oscillator(&oscillator_3_name);
  // oscillator_1->depends.push_back(oscillator_3);
  // oscillator_1->modulator = oscillator_3;
  // oscillator_3->frequency = 20;
  // oscillator_1->fm_on = 1;
  // modules.push_back(oscillator_3);
}

/*
 * Add two signals
 */
void add_signals(vector<float> *buffer1, vector<float> *buffer2, vector<float> *result_buffer, int num_samples)
{
  // For each sample
  for(int i = 0; i < num_samples; i ++)
  {
    // Sum the samples from the two input buffers into the result buffer
    (*result_buffer)[i] = (*buffer1)[i] + (*buffer2)[i];
  }
}

/*
 * Multiply two signals
 */
void multiply_signals(vector<float> *buffer1, vector<float> *buffer2, vector<float> *result_buffer, int num_samples)
{
  // For each sample
  for(int i = 0; i < num_samples; i ++)
  {
    // Sum the samples from the two input buffers into the result buffer
    (*result_buffer)[i] = (*buffer1)[i] * (*buffer2)[i];
  }
}