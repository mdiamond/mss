/*
 * Matthew Diamond 2016
 * The header file for an abstract base class that declares a member function
 * for handling user interactions with graphics objects. In order to write a
 * class that can be notified of interactions with graphics objects, it should
 * be made to inherit from this class and implement its handle_event()
 * function.
 */

#ifndef GRAPHICS_LISTENER_HPP
#define GRAPHICS_LISTENER_HPP

// Included classes
#include "Graphics_Object.hpp"

class Graphics_Listener
{
public:
    // Classes that inherit from this class must implement this function in
    // order to be made aware of user interactions with graphics objects.
    virtual bool handle_event(Graphics_Object *) = 0;
};

#endif

