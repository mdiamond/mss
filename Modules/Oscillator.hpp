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

/***************************
 * TYPES OF WAVEFORMS ENUM *
 ***************************/

enum Waveforms
{
    SIN = 0,
    TRI,
    SAW,
    SQR
};

/************************************
 * OSCILLATOR GRAPHICS OBJECTS ENUM *
 ************************************/

enum Oscillator_Graphics_Objects
{
    OSCILLATOR_OUTPUT_WAVEFORM = 3,
    OSCILLATOR_FREQUENCY_TEXT,
    OSCILLATOR_FREQUENCY_TEXT_BOX,
    OSCILLATOR_PHASE_OFFSET_TEXT,
    OSCILLATOR_PHASE_OFFSET_TEXT_BOX,
    OSCILLATOR_PULSE_WIDTH_TEXT,
    OSCILLATOR_PULSE_WIDTH_TEXT_BOX,
    OSCILLATOR_RANGE_TEXT,
    OSCILLATOR_RANGE_LOW_TEXT_BOX,
    OSCILLATOR_RANGE_HIGH_TEXT_BOX
};

/********************************
 * OSCILLATOR DEPENDENCIES ENUM *
 ********************************/

enum Oscillator_Dependencies
{
    OSCILLATOR_FREQUENCY_DEPENDENCY = 0,
    OSCILLATOR_PHASE_OFFSET_DEPENDENCY,
    OSCILLATOR_PULSE_WIDTH_DEPENDENCY,
    OSCILLATOR_RANGE_LOW_DEPENDENCY,
    OSCILLATOR_RANGE_HIGH_DEPENDENCY
};

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

class Oscillator: public Module
{
    public:
        // The current phase of the oscillator
        float current_phase;
        // Set up floats, buffers, and strings to represent the various
        // characteristics of the oscillator:
        //   - frequency: the frequency at which it oscillates
        //   - phase_offset: what point in oscillation at which to begin oscillating
        //   - pulse_width: the width of the square wave pulse
        //   - range_low: the low point of oscillation
        //   - range_high: the high point of oscillation
        float frequency, phase_offset, pulse_width, range_low, range_high;
        std::vector<float> *input_frequency, *input_phase_offset, *input_pulse_width,
                            *input_range_low, *input_range_high;
        std::string frequency_str, phase_offset_str, pulse_width_str, range_low_str,
                    range_high_str;
        bool live_frequency, live_phase_offset, live_pulse_width, live_range_low,
             live_range_high;
        // Booleans to represent whether or not each of the waveforms is enabled
        int waveform_type;

        // Constructor and destructor
        Oscillator(std::string *, int);
        virtual ~Oscillator();

        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void update_unique_graphics_objects();
        virtual void update_unique_control_values();
        void set_frequency();
        void set_phase_offset();
        void set_pulse_width();
        void set_range_low();
        void set_range_high();
};

#endif
