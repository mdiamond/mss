/*
 * Matthew Diamond 2017
 * The input module. This module simply makes input from audio sources
 * (microphones, etc.) available as signals within the context of this program.
 */

#ifndef MSS_INPUT_HPP
#define MSS_INPUT_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/**************************
 * INPUT CLASS DEFINITION *
 **************************/

class Input: public Module
{
public:
    int SDL_input_number;

    // Constructor and destructor
    Input();
    virtual ~Input();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual bool handle_event(Graphics_Object *);
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
};

#endif

