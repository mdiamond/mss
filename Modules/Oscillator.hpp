/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other oscillator. The
 * resulting waveform can then be processed or output.
 * This module fills its output buffer with the generated signal.
 * This file defines the class.
 */

#ifndef synth_oscillator_h
#define synth_oscillator_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/************************************
 * OSCILLATOR GRAPHICS OBJECTS ENUM *
 ************************************/

enum Oscillator_Graphics_Objects
{
    OSCILLATOR_FREQUENCY_TEXT = 3,
    OSCILLATOR_RESET_CURRENT_PHASE_BUTTON,
    OSCILLATOR_PHASE_OFFSET_TEXT,
    OSCILLATOR_PULSE_WIDTH_TEXT,
    OSCILLATOR_RANGE_TEXT,
    OSCILLATOR_OUTPUT_WAVEFORM,
    OSCILLATOR_FREQUENCY_INPUT_TEXT_BOX,
    OSCILLATOR_PHASE_OFFSET_INPUT_TEXT_BOX,
    OSCILLATOR_PULSE_WIDTH_INPUT_TEXT_BOX,
    OSCILLATOR_RANGE_LOW_INPUT_TEXT_BOX,
    OSCILLATOR_RANGE_HIGH_INPUT_TEXT_BOX,
    OSCILLATOR_FREQUENCY_INPUT_TOGGLE_BUTTON,
    OSCILLATOR_PHASE_OFFSET_INPUT_TOGGLE_BUTTON,
    OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON,
    OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON,
    OSCILLATOR_RANGE_HIGH_INPUT_TOGGLE_BUTTON,
    OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON,
    OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON,
    OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON,
    OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON
};

/********************************
 * OSCILLATOR DEPENDENCIES ENUM *
 ********************************/

enum Oscillator_Dependencies
{
    OSCILLATOR_FREQUENCY = 0,
    OSCILLATOR_PHASE_OFFSET,
    OSCILLATOR_PULSE_WIDTH,
    OSCILLATOR_RANGE_LOW,
    OSCILLATOR_RANGE_HIGH
};

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

class Oscillator: public Module
{
    public:
        // The current phase of the oscillator
        float current_phase;
        // Booleans to represent whether or not each of the waveforms is enabled
        int waveform_type;
        // A record of the previous phase offset value
        float previous_phase_offset;
        // Whether or not each waveform is in use
        bool sin_on, tri_on, saw_on, sqr_on;

        // Constructor and destructor
        Oscillator();
        virtual ~Oscillator();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();

        // Member functions particular to this module
        //   Produce samples for the various types of waveforms given a phase
        float produce_sin_sample(float);
        float produce_tri_sample(float);
        float produce_saw_sample(float);
        float produce_sqr_sample(float);
        //   Switch to outputting the given waveform type
        void switch_waveform(int);
        //   Reset phase
        void reset_current_phase();
};

#endif
