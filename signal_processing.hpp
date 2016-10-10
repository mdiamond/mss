/*
 * Matthew Diamond 2015
 * Header file for signal processing functions, callback functions, and
 * audio related initialization functions.
 */

#ifndef SYNTH_SIGNAL_PROCESSING_HPP
#define SYNTH_SIGNAL_PROCESSING_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Audio and k rate callback functions
void audio_callback(void *, Uint8 *, int);
Uint32 k_rate_callback_function(Uint32, void *);

// Signal processing functions
void clip_signal(std::vector<float> *, float, float);
void copy_signal(std::vector<float> *, std::vector<float> *);
void scale_signal(std::vector<float> *, float, float, float, float);
float scale_sample(float, float, float, float, float);
void add_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *);
void multiply_signals(std::vector<float> *, std::vector<float> *, std::vector<float> *);
void multiply_signals(std::vector<float> *, float, std::vector<float> *);

#endif
