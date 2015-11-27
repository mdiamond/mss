/*
 * Matthew Diamond 2015
 * The ADSR module. This module is capable of generating an
 * envelope given a 1 or 0 as input. A 1 represents note on,
 * resulting in a rise to initial volume over the attack time,
 * then a decay to sustain volume over the decay time, and then
 * sustain at the sustain volume. A 0 represents note off, resulting
 * in a release to no volume over the release time.
 */

#ifndef synth_adsr_h
#define synth_adsr_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/******************************
 * ADSR GRAPHICS OBJECTS ENUM *
 ******************************/

enum Adsr_Graphics_Objects
{
    ADSR_A_D_TEXT = 4,
    ADSR_S_R_TEXT,
    ADSR_NOTE_TEXT,
    ADSR_OUTPUT_WAVEFORM,
    ADSR_A_INPUT_TEXT_BOX,
    ADSR_D_INPUT_TEXT_BOX,
    ADSR_S_INPUT_TEXT_BOX,
    ADSR_R_INPUT_TEXT_BOX,
    ADSR_NOTE_INPUT_TEXT_BOX,
    ADSR_A_INPUT_TOGGLE_BUTTON,
    ADSR_D_INPUT_TOGGLE_BUTTON,
    ADSR_S_INPUT_TOGGLE_BUTTON,
    ADSR_R_INPUT_TOGGLE_BUTTON,
    ADSR_NOTE_INPUT_TOGGLE_BUTTON,
};

/**************************
 * ADSR DEPENDENCIES ENUM *
 **************************/

enum Adsr_Dependencies
{
    ADSR_A = 0,
    ADSR_D,
    ADSR_S,
    ADSR_R,
    ADSR_NOTE
};

/********************
 * ADSR PHASES ENUM *
 ********************/

enum Adsr_Phases
{
    ADSR_A_PHASE = 0,
    ADSR_D_PHASE,
    ADSR_S_PHASE,
    ADSR_R_PHASE
};

/*************************
 * ADSR CLASS DEFINITION *
 *************************/

class Adsr: public Module
{
    public:
        // The current amplitudes
        float current_amplitude;
        // Which phase of the envelope this module is in
        float phase_num;

        // Constructor and destructor
        Adsr();
        virtual ~Adsr();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();
};

#endif
