/*
 * Matthew Diamond 2015
 * Header file for the basic signal processing functions and
 * some callback function helpers. This file declares
 * any functions.
 */

#ifndef synth_signal_processing_h
#define synth_signal_processing_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Initialization functions
int open_audio_device();
void initialize_output();

// Audio callback function
void audio_callback(void *, Uint8 *, int);
void toggle_audio_on();

// Signal processing functions
void add_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);
void multiply_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);

#endif
