/*
 * Matthew Diamond 2015
 * Header file for the basic signal processing functions and
 * some callback function helpers.
 */

#ifndef synth_signal_processing_h
#define synth_signal_processing_h

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Oscillator.hpp"
#include "Output.hpp"

void audio_callback(void *, Uint8 *, int);

int open_audio_device(void);

void initialize_output();

void add_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);
void multiply_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);

#endif