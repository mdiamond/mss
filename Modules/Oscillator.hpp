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

#ifndef SYNTH_OSCILLATOR_HPP
#define SYNTH_OSCILLATOR_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/************************************
 * OSCILLATOR GRAPHICS OBJECTS ENUM *
 ************************************/

enum OscillatorGraphicsObjects
{
    OSCILLATOR_RESET_CURRENT_PHASE_BUTTON = 3,
    OSCILLATOR_FREQUENCY_TEXT,
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

enum OscillatorDependencies
{
    OSCILLATOR_FREQUENCY = 0,
    OSCILLATOR_PHASE_OFFSET,
    OSCILLATOR_PULSE_WIDTH,
    OSCILLATOR_RANGE_LOW,
    OSCILLATOR_RANGE_HIGH
};

/*****************************
 * OSCILLATOR WAVEFORMS ENUM *
 *****************************/

enum WaveformType
{
    SIN = 0,
    TRI,
    SAW,
    SQR
};

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

class Oscillator: public Module
{
public:
    // The current phase of the oscillator
    double current_phase;
    // A record of the previous phase offset value
    double previous_phase_offset;
    // Booleans to represent whether or not each of the waveforms is enabled
    WaveformType waveform_type;
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
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);

    // Member functions particular to this module
    //   Produce samples for the various types of waveforms given a phase
    double produce_sin_sample(double);
    double produce_tri_sample(double);
    double produce_saw_sample(double);
    double produce_sqr_sample(double);
    //   Switch to outputting the given waveform type
    void switch_waveform(WaveformType);
    //   Reset phase
    void reset_current_phase();
};

#endif
