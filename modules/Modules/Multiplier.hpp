/*
 * Matthew Diamond 2015
 * The multiplier module. This module applies control values to another signal.
 * Essentially, it just multiplies two signals, but it is also possible to
 * specify an amount to apply for the control values signal.
 */

#ifndef MSS_MULTIPLIER_HPP
#define MSS_MULTIPLIER_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*******************************
 * MULTIPLIER CLASS DEFINITION *
 *******************************/

class Multiplier: public Module
{
public:
    // Multiplier graphics objects enum
    enum MultiplierGraphicsObjects
    {
        MULTIPLIER_INPUT_TEXT = 3,
        MULTIPLIER_DRY_WET_TEXT,
        MULTIPLIER_OUTPUT_WAVEFORM,
        MULTIPLIER_SIGNAL_INPUT_TEXT_BOX,
        MULTIPLIER_MULTIPLIER_INPUT_TEXT_BOX,
        MULTIPLIER_DRY_WET_INPUT_TEXT_BOX,
        MULTIPLIER_SIGNAL_INPUT_TOGGLE_BUTTON,
        MULTIPLIER_MULTIPLIER_INPUT_TOGGLE_BUTTON,
        MULTIPLIER_DRY_WET_INPUT_TOGGLE_BUTTON
    };

    // Multiplier dependencies enum
    enum MultiplierDependencies
    {
        MULTIPLIER_SIGNAL = 0,
        MULTIPLIER_MULTIPLIER,
        MULTIPLIER_DRY_WET
    };

    // Constructor and destructor
    Multiplier();
    virtual ~Multiplier();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);
};

#endif

