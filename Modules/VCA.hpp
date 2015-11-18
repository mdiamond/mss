/*
 * Matthew Diamond 2015
 * The VCA module. This module applies control values
 * to another signal. Essentially, it just multiplies two signals,
 * but it is also possible to specify an amount to apply for the
 * control values signal.
 */

#ifndef synth_vca_h
#define synth_vca_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/*****************************
 * VCA GRAPHICS OBJECTS ENUM *
 *****************************/

enum VCA_Graphics_Objects
{
    VCA_INPUT_TEXT = 3,
    VCA_CV_AMOUNT_TEXT,
    VCA_OUTPUT_WAVEFORM,
    VCA_SIGNAL_INPUT_TEXT_BOX,
    VCA_CV_INPUT_TEXT_BOX,
    VCA_CV_AMOUNT_INPUT_TEXT_BOX,
    VCA_SIGNAL_INPUT_TOGGLE_BUTTON,
    VCA_CV_INPUT_TOGGLE_BUTTON,
    VCA_CV_AMOUNT_INPUT_TOGGLE_BUTTON
};

/*************************
 * VCA DEPENDENCIES ENUM *
 *************************/

enum VCA_Dependencies
{
    VCA_SIGNAL = 0,
    VCA_CV,
    VCA_CV_AMOUNT
};

/************************
 * VCA CLASS DEFINITION *
 ************************/

class Vca: public Module
{
    public:
        // Set up floats, buffers, and strings to represent the various
        // characteristics of the VCA module:
        //   - signal: the incoming signal to be controlled
        //   - cv: the control values to apply to the signal
        //   - cv_amount: how much of the modulation to apply
        float signal_float, cv_float, cv_amount_float;
        std::vector<float> *signal_input, *cv_input, *cv_amount_input;
        std::string signal_str, cv_str, cv_amount_str;

        // Constructor and destructor
        Vca();
        virtual ~Vca();

        // Member functions
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
};

#endif
