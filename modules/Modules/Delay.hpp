/*
 * Matthew Diamond 2015
 * The delay module. This module is capable of applying a delay line to a
 * signal, complete with wet/dry and feedback amount controls.
 */

#ifndef MSS_DELAY_HPP
#define MSS_DELAY_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/**************************
 * DELAY CLASS DEFINITION *
 **************************/

class Delay: public Module
{
public:
    // Delay dependencies enum
    enum DelayDependencies
    {
        DELAY_SIGNAL = 0,
        DELAY_MAX_DELAY_TIME,
        DELAY_DELAY_TIME,
        DELAY_FEEDBACK_AMOUNT,
        DELAY_WET_DRY
    };

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
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();

    // Member functions particular to this module
    //   Calculate a linearly interpolated wet sample from
    //   the cirular buffer
    float calculate_wet_sample();
    //   Reset buffer
    void reset_buffer();
};

#endif

