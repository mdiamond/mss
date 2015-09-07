/*
 * Matthew Diamond 2015
 * Header file for the basic signal processing functions and
 * some callback function helpers.
 */

#ifndef synth_signal_processing_h
#define synth_signal_processing_h

// Included libraries
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

void fill_buffer(Uint8 *, int);

int open_audio_device(void);

void add_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);

#endif