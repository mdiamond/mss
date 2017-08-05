/*
 * Matthew Diamond 2015
 * Header file for the function forwarder.
 */

#ifndef MSS_FUNCTION_FORWARDER_HPP
#define MSS_FUNCTION_FORWARDER_HPP

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included graphics classes
#include "Graphics_Listener.hpp"
#include "Graphics_Object.hpp"

/***************************************
 * FUNCTION FORWARDER CLASS DEFINITION *
 ***************************************/

class Function_Forwarder: public Graphics_Listener
{
public:
    // Constructor and destructor
    Function_Forwarder();
    virtual ~Function_Forwarder();

    // Member functions
    bool handle_event(Graphics_Object *);

private:
    std::vector<std::string> possible_names =
        {"add adsr button", "add delay button", "add filter button",
         "add mixer button", "add multiplier button", "add noise button",
         "add oscillator button", "add sah button", "previous page button",
         "next page button", "save patch text box", "load patch text box"
        };
};

#endif

