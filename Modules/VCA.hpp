/*
 * Matthew Diamond 2015
 * The VCA module. This module applies control values
 * to another signal. Essentially, it just multiplies two signals,
 * but it is also possible to specify a scale factor for the
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
    VCA_OUTPUT_WAVEFORM = 3,
    VCA_INPUT_TEXT,
    VCA_SIGNAL_TEXT_BOX,
    VCA_CV_TEXT_BOX,
    VCA_CV_SCALE_TEXT,
    VCA_CV_SCALE_TEXT_BOX
};

/*************************
 * VCA DEPENDENCIES ENUM *
 *************************/

enum VCA_Dependencies
{
    VCA_SIGNAL_DEPENDENCY = 0,
    VCA_CV_DEPENDENCY,
    VCA_CV_SCALE_DEPENDENCY
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
        //   - cv_scale: a scale factor to apply to the cv values
        float signal_float, cv_float, cv_amount_float;
        std::vector<float> *signal_input, *cv_input, *cv_amount_input;
        std::string signal_str, cv_str, cv_amount_str;

        // Constructor and destructor
        Vca(std::string, int);
        virtual ~Vca();

        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void update_unique_graphics_objects();
        virtual void update_unique_control_values();
        void set_signal(Module *);
        void set_cv(Module *);
        void set_cv_amount(float);
        void set_cv_amount(Module *);
};

#endif
