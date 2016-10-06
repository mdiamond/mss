/*
 * Matthew Diamond 2015
 * The delay module. This module is capable of applying a
 * delay line to a signal, complete with wet/dry and feedback amount
 * controls.
 */

#ifndef SYNTH_DELAY_HPP
#define SYNTH_DELAY_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*******************************
 * DELAY GRAPHICS OBJECTS ENUM *
 *******************************/

enum Delay_Graphics_Objects
{
    DELAY_RESET_BUFFER_BUTTON = 3,
    DELAY_SIGNAL_TEXT,
    DELAY_MAX_DELAY_TIME_AND_DELAY_TIME_TEXT,
    DELAY_FEEDBACK_AMOUNT_AND_WET_DRY_TEXT,
    DELAY_OUTPUT_WAVEFORM,
    DELAY_SIGNAL_INPUT_TEXT_BOX,
    DELAY_MAX_DELAY_TIME_INPUT_TEXT_BOX,
    DELAY_DELAY_TIME_INPUT_TEXT_BOX,
    DELAY_FEEDBACK_AMOUNT_INPUT_TEXT_BOX,
    DELAY_WET_DRY_INPUT_TEXT_BOX,
    DELAY_SIGNAL_INPUT_TOGGLE_BUTTON,
    DELAY_DELAY_TIME_INPUT_TOGGLE_BUTTON,
    DELAY_FEEDBACK_AMOUNT_INPUT_TOGGLE_BUTTON,
    DELAY_WET_DRY_INPUT_TOGGLE_BUTTON,
};

/***************************
 * DELAY DEPENDENCIES ENUM *
 ***************************/

enum Delay_Dependencies
{
    DELAY_SIGNAL = 0,
    DELAY_MAX_DELAY_TIME,
    DELAY_DELAY_TIME,
    DELAY_FEEDBACK_AMOUNT,
    DELAY_WET_DRY
};

/**************************
 * DELAY CLASS DEFINITION *
 **************************/

class Delay: public Module
{
public:
    // Delayed buffer
    std::vector<float> circular_buffer;
    double current_sample;
    double delay_samples;
    // Previous max delay time
    float previous_max_delay_time;
    float previous_delay_time;

    // Constructor and destructor
    Delay();
    virtual ~Delay();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual void update_control_values();
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);

    // Member functions particular to this module
    //   Calculate a linearly interpolated wet sample from
    //   the cirular buffer
    float calculate_wet_sample();
    //   Reset buffer
    void reset_buffer();
};

#endif
