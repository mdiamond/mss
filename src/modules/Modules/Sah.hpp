/*
 * Matthew Diamond 2015
 * The multiplier module. This module applies control values
 * to another signal. Essentially, it just multiplies two signals,
 * but it is also possible to specify an amount to apply for the
 * control values signal.
 */

#ifndef MSS_SAH_HPP
#define MSS_SAH_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/************************
 * SAH CLASS DEFINITION *
 ************************/

class Sah: public Module
{
public:
    // Sample and hold dependencies enum
    enum SahDependencies
    {
        SAH_SIGNAL = 0,
        SAH_HOLD_TIME
    };

    double sample;
    double time_to_next_sample;

    // Constructor and destructor
    Sah();
    virtual ~Sah();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();

    // Member functions particular to this module
    //   Reset phase
    void reset_sampler();
};

#endif

