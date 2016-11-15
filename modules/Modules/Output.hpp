/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback function comes through
 * here. This module simply processes the modules it depends upon, then makes
 * available full audio buffers to the audio callback function. This file
 * defines the class.
 */

#ifndef MSS_OUTPUT_HPP
#define MSS_OUTPUT_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/***************************
 * OUTPUT CLASS DEFINITION *
 ***************************/

class Output: public Module
{
public:
    // Output dependencies enum
    enum OuputDependencies
    {
        OUTPUT_INPUT_L = 0,
        OUTPUT_INPUT_R
    };

    // Constructor and destructor
    Output();
    virtual ~Output();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();

    // Member functions particular to this module
    //   Toggle audio processing
    void toggle_audio();
};

#endif

