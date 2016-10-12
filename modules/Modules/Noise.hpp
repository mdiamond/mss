/*
 * Matthew Diamond 2015
 * The noise module. This module is capable of generating different types of
 * noise.
 */

#ifndef MSS_NOISE_HPP
#define MSS_NOISE_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/**************************
 * NOISE CLASS DEFINITION *
 **************************/

class Noise: public Module
{
public:
    // Noise graphics objects enum
    enum NoiseGraphicsObjects
    {
        NOISE_OUTPUT_WAVEFORM = 3,
        NOISE_RANGE_TEXT,
        NOISE_RANGE_LOW_INPUT_TEXT_BOX,
        NOISE_RANGE_HIGH_INPUT_TEXT_BOX,
        NOISE_RANGE_LOW_INPUT_TOGGLE_BUTTON,
        NOISE_RANGE_HIGH_INPUT_TOGGLE_BUTTON,
    };

    // Noise dependencies enum
    enum NoiseDependencies
    {
        NOISE_RANGE_LOW = 0,
        NOISE_RANGE_HIGH
    };

    // Constructor and destructor
    Noise();
    virtual ~Noise();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);

    // Member functions particular to this module
    //   Produce samples for the various types of waveforms given a phase
    double produce_white_noise_sample();
    //   Switch to outputting the given waveform type
    void switch_noise_type(int);
};

#endif

