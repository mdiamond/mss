/*
 * Matthew Diamond 2015
 * Header file for the driver functions.
 */

#ifndef synth_main_h
#define synth_main_h

// Included SDL components
#include "SDL2/SDL.h"

extern int SAMPLE_RATE;
extern unsigned long long int CURRENT_SAMPLE;
extern int AUDIO_LENGTH;
extern float FREQUENCY;

void audio_callback(void *, Uint8 *, int);

#endif
