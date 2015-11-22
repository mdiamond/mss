/*
 * Matthew Diamond 2015
 * Signal processing functions, callback functions, and
 * audio related initialization functions.
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
        std::cout << RED_STDOUT << "Could not open the audio device: "
             << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Audio device opened." << std::endl;
    std::cout << "Audio details:" << std::endl;
    std::cout << "    Sample rate: " << obtained.freq << std::endl;
    std::cout << "    Format: " << obtained.format << std::endl;
    std::cout << "    Channels: " << obtained.channels << std::endl;
    std::cout << "    Buffer size in samples: " << obtained.samples << std::endl;
    std::cout << "    Buffer size in bytes: " << obtained.size << std::endl;

    BUFFER_SIZE = obtained.samples;

    // Return success
    return 1;
}

/*
 * Initialize the one and only output module.
 */
void initialize_output()
{
    // Create the output module
    Output *output = new Output();
    output->initialize_graphics_objects();
    MODULES.push_back(output);

    std::cout << "Output initialized." << std::endl;
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
    Output *output = (Output *) MODULES[0];

    // Process audio for the output module
    // This will recursively call upon depended modules
    // for processed audio, meaning that modules at
    // the beginning of the signal chain will be processed first
    output->process();

    // Fetch the output module's latest processed audio
    // and insert it into the buffer
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        if((output->inputs)[OUTPUT_INPUT_L] != NULL)
            buffer[0] = (output->inputs)[OUTPUT_INPUT_L]->at(i);
        else
            buffer[0] = 0;
        if((output->inputs)[OUTPUT_INPUT_R] != NULL)
            buffer[1] = (output->inputs)[OUTPUT_INPUT_R]->at(i);
        else
            buffer[1] = 0;

        buffer += 2;
    }

    for(unsigned int i = 1; i < MODULES.size(); i ++)
        MODULES[i]->processed = false;
}

/*
 * K rate callback is triggered by an SDL_Timer, and simply updates
 * control values for all modules at a rate specified in main.cpp.
 */
Uint32 k_rate_callback_function(Uint32 interval, void *param)
{
    if(SDL_GetAudioStatus() == SDL_AUDIO_PAUSED)
        return 0;

    for(unsigned int i = 0; i < MODULES.size(); i ++)
        MODULES[i]->update_control_values();

    return interval;
}

/*******************************
 * SIGNAL PROCESSING FUNCTIONS *
 *******************************/

/*
 * Clip a signal within its buffer.
 * Make sure no value goes above the max,
 * and no value goes below the min.
 */
void clip_signal(std::vector<float> *buffer, float min, float max)
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
void copy_signal(std::vector<float> *src, std::vector<float> *dst)
{
    for(unsigned int i = 0; i < src->size(); i ++)
        (*dst)[i] = (*src)[i];
}

/*
 * Scale a signal within its buffer.
 */
void scale_signal(std::vector<float> *buffer, float original_low,
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
 * Scale a sample and return the scaled sample.
 */
float scale_sample(float sample, float original_low,
                  float original_high, float low, float high)
{
    sample = (sample - original_low) / (original_high - original_low);
    sample *= high - low;
    sample += low;

    return sample;
}

/*
 * Add two signals into a destination buffer.
 */
void add_signals(std::vector<float> *buffer1, std::vector<float> *buffer2, std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer1->size(); i ++)
        (*dst)[i] = (*buffer1)[i] + (*buffer2)[i];
}

/*
 * Multiply two signals into a destination buffer.
 */
void multiply_signals(std::vector<float> *buffer1, std::vector<float> *buffer2, std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer1->size(); i ++)
        (*dst)[i] = (*buffer1)[i] * (*buffer2)[i];
}

/*
 * Multiply a signal by some constant and put the resulting signal
 * into a destination buffer.
 */
void multiply_signals(std::vector<float> *buffer, float val, std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer->size(); i ++)
        (*dst)[i] = (*buffer)[i] * val;
}
