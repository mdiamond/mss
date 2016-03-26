/*
 * Matthew Diamond 2015
 * The filter module.
 */

#ifndef synth_filter_h
#define synth_filter_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/********************************
 * FILTER GRAPHICS OBJECTS ENUM *
 ********************************/

enum Filter_Graphics_Objects
{
    FILTER_SIGNAL_TEXT = 3,
    FILTER_FREQUENCY_CUTOFF_AND_Q_TEXT,
    FILTER_OUTPUT_WAVEFORM,
    FILTER_SIGNAL_INPUT_TEXT_BOX,
    FILTER_FREQUENCY_CUTOFF_INPUT_TEXT_BOX,
    FILTER_Q_INPUT_TEXT_BOX,
    FILTER_SIGNAL_INPUT_TOGGLE_BUTTON,
    FILTER_FREQUENCY_CUTOFF_INPUT_TOGGLE_BUTTON,
    FILTER_Q_INPUT_TOGGLE_BUTTON,
    FILTER_LP_TOGGLE_BUTTON,
    FILTER_BP_TOGGLE_BUTTON,
    FILTER_HP_TOGGLE_BUTTON,
};

/****************************
 * FILTER DEPENDENCIES ENUM *
 ****************************/

enum Filter_Dependencies
{
    FILTER_SIGNAL = 0,
    FILTER_FREQUENCY_CUTOFF,
    FILTER_Q,
};

/*********************
 * FILTER TYPES ENUM *
 *********************/

enum Filters
{
    LOWPASS = 0,
    BANDPASS,
    HIGHPASS
};

/***************************
 * FILTER CLASS DEFINITION *
 ***************************/

class Filter: public Module
{
    // Vectors to track samples passing through and the impulse response
    std::vector<float> iir_samples, iir_coefficients;
    double y1, y2, x1, x2;
    int filter_type;
    bool lowpass_on, bandpass_on, highpass_on;

    public:
        // Booleans to represent whether or not each of the waveforms is enabled
        // int waveform_type;
        // Whether or not each waveform is in use
        // bool sin_on, tri_on, saw_on, sqr_on;

        // Constructor and destructor
        Filter();
        virtual ~Filter();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();
        virtual void button_function(Button *);
        virtual void toggle_button_function(Toggle_Button *);

        // Member functions particular to this module
        //   Switch to the given waveform type
        void switch_filter(int);
};

#endif
