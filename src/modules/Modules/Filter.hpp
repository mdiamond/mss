/*
 * Matthew Diamond 2015
 * The filter module.
 */

#ifndef MSS_FILTER_HPP
#define MSS_FILTER_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/***************************
 * FILTER CLASS DEFINITION *
 ***************************/

class Filter: public Module
{
public:
    // Filter type enum
    enum FilterType
    {
        LOWPASS = 0,
        BANDPASS,
        HIGHPASS
    };

    // Filter dependencies enum
    enum FilterDependencies
    {
        FILTER_SIGNAL = 0,
        FILTER_FREQUENCY_CUTOFF,
        FILTER_Q,
    };

    // Vectors to track samples passing through and the impulse response
    std::vector<float> iir_samples, iir_coefficients;
    double y1, y2, x1, x2;
    FilterType filter_type;
    bool lowpass_on, bandpass_on, highpass_on;

    // Constructor and destructor
    Filter();
    virtual ~Filter();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();

    // Member functions particular to this module
    //   Switch to the given waveform type
    void switch_filter(FilterType);
};

#endif

