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

// Audio and k rate callback functions
void audio_callback(void *, Uint8 *, int);
Uint32 k_rate_callback_function(Uint32, void *);

// Signal processing functions
void clip_signal(std::vector<float> *, float, float);
void copy_signal(std::vector<float> *, std::vector<float> *);
void scale_signal(std::vector<float> *, float, float, float, float);
void add_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);
void multiply_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *, int);

#endif
