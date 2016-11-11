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
#include "SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

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
void audio_callback(void *userdata_, Uint8 *buffer_, int length_)
{
    // Cast the buffer to a float buffer
    float (*buffer)[NUM_CHANNELS] = (float (*)[NUM_CHANNELS]) buffer_;

    // Get the address of the output module for later use
    Output *output = (Output *) MODULES[0];

    // Process audio for the output module
    // This will recursively call upon depended modules
    // for processed audio, meaning that modules at
    // the beginning of the signal chain will be processed first
    output->process();

    for(unsigned int i = 0; i < BUFFER_SIZE; i ++)
    {
        for(unsigned int j = 0; j < NUM_CHANNELS; j ++)
        {
            buffer[i][j] = output->inputs[j].live
                           ? output->inputs[j].in->at(i) : 0;
        }
    }

    for(unsigned int i = 1; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            MODULES[i]->processed = false;
        }
    }

    // for(unsigned int i = 1; i < MODULES.size(); i ++)
    // {
    //     if(MODULES[i] != NULL)
    //     {
    //         for(int j = 0; j < BUFFER_SIZE; j ++)
    //         {
    //             if(MODULES[i]->output[j] > 1 || MODULES[i]->output[j] < -1)
    //             {
    //                 std::cout << RED_STDOUT << MODULES[i]->name << " is clipping" << DEFAULT_STDOUT << std::endl;
    //                 break;
    //             }
    //         }
    //     }
    // }
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
        {
            (*buffer)[i] = max;
        }
        else if((*buffer)[i] < min)
        {
            (*buffer)[i] = min;
        }
    }
}

/*
 * Copy a signal to a different buffer.
 */
void copy_signal(std::vector<float> *src, std::vector<float> *dst)
{
    for(unsigned int i = 0; i < src->size(); i ++)
    {
        (*dst)[i] = (*src)[i];
    }
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
void add_signals(std::vector<float> *buffer1, std::vector<float> *buffer2,
                 std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer1->size(); i ++)
    {
        (*dst)[i] = (*buffer1)[i] + (*buffer2)[i];
    }
}

/*
 * Multiply two signals into a destination buffer.
 */
void multiply_signals(std::vector<float> *buffer1, std::vector<float> *buffer2,
                      std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer1->size(); i ++)
    {
        (*dst)[i] = (*buffer1)[i] * (*buffer2)[i];
    }
}

/*
 * Multiply a signal by some constant and put the resulting signal
 * into a destination buffer.
 */
void multiply_signals(std::vector<float> *buffer, float val,
                      std::vector<float> *dst)
{
    // For each sample
    for(unsigned int i = 0; i < buffer->size(); i ++)
    {
        (*dst)[i] = (*buffer)[i] * val;
    }
}

