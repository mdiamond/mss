/*
 * Matthew Diamond 2015
 * The NOISE module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other NOISE. The
 * resulting waveform can then be processed or output.
 * This module fills its output buffer with the generated signal.
 * This file defines the class.
 */

#ifndef synth_noise_h
#define synth_noise_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/************************************
 * NOISE GRAPHICS OBJECTS ENUM *
 ************************************/

enum Noise_Graphics_Objects
{
    NOISE_OUTPUT_WAVEFORM = 3,
    // NOISE_SIN_WAVE_TOGGLE_BUTTON,
    // NOISE_TRI_WAVE_TOGGLE_BUTTON,
    // NOISE_SAW_WAVE_TOGGLE_BUTTON,
    // NOISE_SQR_WAVE_TOGGLE_BUTTON
};

/*******************************
 * NOISE CLASS DEFINITION *
 *******************************/

class Noise: public Module
{
    public:
        // Noise type
        int noise_type;

        // Constructor and destructor
        Noise();
        virtual ~Noise();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();

        // Member functions particular to this module
        //   Produce samples for the various types of waveforms given a phase
        double produce_white_noise_sample();
        //   Switch to outputting the given waveform type
        void switch_noise_type(int);
};

#endif
