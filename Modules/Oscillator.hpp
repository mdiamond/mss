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
    OSCILLATOR_RANGE_HIGH_TEXT_BOX,
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
        // Booleans to represent whether or not each of the waveforms is enabled
        int waveform_type;
        // A pointer to the wavetable currently in use
        std::vector<float> *wavetable;
        // A record of the previous phase offset value
        float previous_phase_offset;
        // Whether or not each waveform is in use
        bool sin_on, tri_on, saw_on, sqr_on;

        // Constructor and destructor
        Oscillator(std::string, int);
        virtual ~Oscillator();

        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void update_unique_graphics_objects();
        virtual void update_unique_control_values();
        float produce_sin_sample(float);
        float produce_tri_sample(float);
        float produce_saw_sample(float);
        float produce_sqr_sample(float);
        void set_frequency(float);
        void set_frequency(Module *);
        void set_phase_offset(float);
        void set_phase_offset(Module *);
        void set_pulse_width(float);
        void set_pulse_width(Module *);
        void set_range_low(float);
        void set_range_low(Module *);
        void set_range_high(float);
        void set_range_high(Module *);
        void switch_waveform(int);
};

#endif
