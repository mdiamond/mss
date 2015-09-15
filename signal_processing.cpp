/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer and helper functions
 * for main().
 */

// Include header file
#include "signal_processing.hpp"

using namespace std;

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open the audio device with a simple configuration.
 */
int open_audio_device()
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

  cout << "Audio device opened." << endl;
  cout << "Audio details:" << endl;
  cout << "  Sample rate: " << obtained.freq << endl;
  cout << "  Format: " << obtained.format << endl;
  cout << "  Channels: " << obtained.channels << endl;
  cout << "  Buffer size in samples: " << obtained.samples << endl;
  cout << "  Buffer size in bytes: " << obtained.size << endl;

  BUFFER_SIZE = obtained.samples;

  // Return success
  return 1;
}

void initialize_output()
{
  // Create the output module
  Output *output = new Output();
  MODULES.push_back(output);

  // This configuration is arbitrary and
  // it will be possible to patch it together
  // in the GUI later
  // In the GUI version, this function will only
  // create the output module

  // Create an oscillator module
  string oscillator_1_name = "oscillator 1";
  Oscillator *oscillator_1 = new Oscillator(&oscillator_1_name);
  MODULES.push_back(oscillator_1);

  // Set the oscillator frequencies
  (oscillator_1->audio).frequency = 440;

  // Create another oscillator module
  // string modulator_name = "modulator";
  // Oscillator *modulator = new Oscillator(&modulator_name);
  // MODULES.push_back(modulator);

  // (modulator->audio).frequency = 4;

  // oscillator_1->depends.push_back(modulator);
  // (oscillator_1->audio).fm_on = 1;
  // (oscillator_1->audio).modulation_index = 25;
  // oscillator_1->modulator = modulator;

  // Set the inputs and outputs
  output->depends.push_back(oscillator_1);
  (output->audio).input_l = (oscillator_1->audio).output;
  (output->audio).input_r = (oscillator_1->audio).output;

  cout << "Output initialized." << endl;
}

/***************************
 * AUDIO CALLBACK FUNCTION *
 ***************************/

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
  Output *output = (Output *) MODULES[0];

  // Process audio for the output module
  // This will recursively call upon depended modules
  // for processed audio, meaning that modules at
  // the beginning of the signal chain will be processed first
  output->process();

  // Fetch the output module's latest processed audio
  // and insert it into the buffer
  float *buffer_l = buffer;
  float *buffer_r = buffer + 1;
  for(int i = 0; i < BUFFER_SIZE; i ++)
  {
    *buffer_l = (*((output->audio).input_l))[i];
    *buffer_r = (*((output->audio).input_r))[i];
    buffer_l += 2;
    buffer_r += 2;
    // Uncomment these for some cool parameter modulation
    // (((Oscillator *)MODULES[1])->audio).frequency += .001;
    // (((Oscillator *)MODULES[2])->audio).frequency += .0001;
    // (((Oscillator *)MODULES[2])->audio).modulation_index -= .00001;
  }

  // Increment the current sample by the number
  // of samples just processed
  CURRENT_SAMPLE += BUFFER_SIZE;
  AUDIO_LENGTH -= BUFFER_SIZE;

  // populate_samples(output->input_l, output->input_r);
}

/*******************************
 * SIGNAL PROCESSING FUNCTIONS *
 *******************************/

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
