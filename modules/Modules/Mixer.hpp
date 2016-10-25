/*
 * Matthew Diamond 2015
 * The mixer module. This module attenuates and then sums a few signals. It is
 * capable of automatically attenuating based on how many channels are active
 * if desired.
 */

#ifndef MSS_MIXER_HPP
#define MSS_MIXER_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/**************************
 * MIXER CLASS DEFINITION *
 **************************/

class Mixer: public Module
{
public:
    // Mixer dependencies enum
    enum MixerDependencies
    {
        MIXER_SIGNAL_1 = 0,
        MIXER_SIGNAL_1_MULTIPLIER,
        MIXER_SIGNAL_2,
        MIXER_SIGNAL_2_MULTIPLIER,
        MIXER_SIGNAL_3,
        MIXER_SIGNAL_3_MULTIPLIER,
        MIXER_SIGNAL_4,
        MIXER_SIGNAL_4_MULTIPLIER,
        MIXER_SIGNAL_5,
        MIXER_SIGNAL_5_MULTIPLIER
    };

    // A boolean to represent whether or not auto attenuation should be used
    bool auto_attenuate;

    // Constructor and destructor
    Mixer();
    virtual ~Mixer();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);
};

#endif

