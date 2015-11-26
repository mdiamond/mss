/*
 * Matthew Diamond 2015
 * The multiplier module. This module applies control values
 * to another signal. Essentially, it just multiplies two signals,
 * but it is also possible to specify an amount to apply for the
 * control values signal.
 */

#ifndef synth_multiplier_h
#define synth_multiplier_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/************************************
 * MULTIPLIER GRAPHICS OBJECTS ENUM *
 ************************************/

enum Multiplier_Graphics_Objects
{
    MULTIPLIER_INPUT_TEXT = 4,
    MULTIPLIER_CV_AMOUNT_TEXT,
    MULTIPLIER_OUTPUT_WAVEFORM,
    MULTIPLIER_SIGNAL_INPUT_TEXT_BOX,
    MULTIPLIER_CV_INPUT_TEXT_BOX,
    MULTIPLIER_CV_AMOUNT_INPUT_TEXT_BOX,
    MULTIPLIER_SIGNAL_INPUT_TOGGLE_BUTTON,
    MULTIPLIER_CV_INPUT_TOGGLE_BUTTON,
    MULTIPLIER_CV_AMOUNT_INPUT_TOGGLE_BUTTON
};

/********************************
 * MULTIPLIER DEPENDENCIES ENUM *
 ********************************/

enum Multiplier_Dependencies
{
    MULTIPLIER_SIGNAL = 0,
    MULTIPLIER_CV,
    MULTIPLIER_CV_AMOUNT
};

/*******************************
 * MULTIPLIER CLASS DEFINITION *
 *******************************/

class Multiplier: public Module
{
    public:
        // Constructor and destructor
        Multiplier();
        virtual ~Multiplier();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();
};

#endif
