/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer and helper functions
 * for main().
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

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
        cout << RED_STDOUT << "Could not open the audio device: "
             << SDL_GetError() << DEFAULT_STDOUT << endl;
        return 0;
    }

    cout << "Audio device opened." << endl;
    cout << "Audio details:" << endl;
    cout << "    Sample rate: " << obtained.freq << endl;
    cout << "    Format: " << obtained.format << endl;
    cout << "    Channels: " << obtained.channels << endl;
    cout << "    Buffer size in samples: " << obtained.samples << endl;
    cout << "    Buffer size in bytes: " << obtained.size << endl;

    BUFFER_SIZE = obtained.samples;

    // Return success
    return 1;
}

void initialize_output()
{
    // Create the output module
    Output *output = new Output(MODULES->size());
    MODULES->push_back(output);

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

    // Get the address of the output module for later use
    Output *output = (Output *) (*MODULES)[0];

    // Process audio for the output module
    // This will recursively call upon depended modules
    // for processed audio, meaning that modules at
    // the beginning of the signal chain will be processed first
    output->process();

    // Fetch the output module's latest processed audio
    // and insert it into the buffer
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        buffer[0] = (*(output->input_l))[i];
        buffer[1] = (*(output->input_r))[i];
        buffer += 2;
    }

    for(unsigned int i = 1; i < MODULES->size(); i ++)
        (*MODULES)[i]->processed = false;
}

/*
 * K rate callback is triggered by an SDL_Timer, and simply updates
 * control values for all modules at 1/100th of a second.
 */
Uint32 k_rate_callback_function(Uint32 interval, void *param)
{
    for(unsigned int i = 0; i < MODULES->size(); i ++)
        (*MODULES)[i]->update_control_values();

    return interval;
}

/*******************************
 * SIGNAL PROCESSING FUNCTIONS *
 *******************************/

/*
 * Clip a signal. Make sure no value goes above the max,
 * and no value goes below the min.
 */
void clip_signal(vector<float> *buffer, float min, float max)
{
    for(unsigned int i = 0; i < buffer->size(); i ++)
    {
        if((*buffer)[i] > max)
            (*buffer)[i] = max;
        else if((*buffer)[i] < min)
            (*buffer)[i] = min;
    }
}

/*
 * Copy a signal to a different buffer.
 */
void copy_signal(vector<float> *src, vector<float> *dst)
{
    for(unsigned int i = 0; i < src->size(); i ++)
        (*dst)[i] = (*src)[i];
}

/*
 * Scale a signal.
 */
void scale_signal(vector<float> *buffer, float original_low,
                  float original_high, float low, float high)
{
    float temp_high = original_high - original_low;
    for(unsigned int i = 0; i < buffer->size(); i ++)
    {
        (*buffer)[i] = ((*buffer)[i] - original_low) / temp_high;
        (*buffer)[i] *= high - low;
        (*buffer)[i] += low;
    }
}

/*
 * Add two signals
 */
void add_signals(vector<float> *buffer1, vector<float> *buffer2, vector<float> *dst, int num_samples)
{
    // For each sample
    for(int i = 0; i < num_samples; i ++)
        (*dst)[i] = (*buffer1)[i] + (*buffer2)[i];
}

/*
 * Multiply two signals
 */
void multiply_signals(vector<float> *buffer1, vector<float> *buffer2, vector<float> *dst, int num_samples)
{
    // For each sample
    for(int i = 0; i < num_samples; i ++)
        (*dst)[i] = (*buffer1)[i] * (*buffer2)[i];
}
