/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of generating a sine, square,
 * triangle, and sawtooth wave given a frequency and pulse width. It can then
 * be modulated with some other oscillator. The resulting waveform can then
 * be processed or output. This module fills its output buffer with the
 * generated signal. This file defines the class.
 */

#ifndef MSS_OSCILLATOR_HPP
#define MSS_OSCILLATOR_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

class Oscillator: public Module
{
public:
    // Oscillator waveform type enum
    enum WaveformType
    {
        SIN = 0,
        TRI,
        SAW,
        SQR
    };

    // Oscillator dependencies enum
    enum OscillatorDependencies
    {
        OSCILLATOR_FREQUENCY = 0,
        OSCILLATOR_PHASE_OFFSET,
        OSCILLATOR_PULSE_WIDTH,
        OSCILLATOR_RANGE_LOW,
        OSCILLATOR_RANGE_HIGH
    };

    // The current phase of the oscillator
    double phase;
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
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();

    // Member functions particular to this module
    //   Produce samples for the various types of waveforms given a phase
    double produce_sin_sample(double);
    double produce_tri_sample(double);
    double produce_saw_sample(double);
    double produce_sqr_sample(double);
    //   Switch to outputting the given waveform type
    void switch_waveform(WaveformType);
    //   Reset phase
    void reset_phase();
};

#endif

