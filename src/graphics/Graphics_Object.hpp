/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic graphics object.
 * Other classes will extend this class and represent different
 * types of graphics objects. This file defines the class.
 * The following classes are derived from the graphics object class:
 *   - Button
 *   - Page
 *   - Rect
 *   - Text
 *   - Text_Box
 *   - Toggle_Button
 *   - Waveform
 */

#ifndef GRAPHICS_GRAPHICS_OBJECT_HPP
#define GRAPHICS_GRAPHICS_OBJECT_HPP

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>

// Included SDL components
#include "SDL.h"

/************************************
 * GRAPHICS_OBJECT CLASS DEFINITION *
 ************************************/

// Forward declaration of graphics listener classe
class Graphics_Listener;

class Graphics_Object
{
public:
    // Graphics_Object type enum
    enum GraphicsObjectType
    {
        BUTTON = 0,
        MODULE,
        PAGE,
        RECT,
        TEXT,
        TEXT_BOX,
        TOGGLE_BUTTON,
        WAVEFORM
    };

    // Module information
    std::string name;
    GraphicsObjectType graphics_object_type;
    SDL_Rect location;
    SDL_Color color;
    bool updated;
    // Constructor and destructor
    Graphics_Object(std::string, GraphicsObjectType graphics_object_type,
                    Graphics_Listener *, SDL_Rect, SDL_Color);
    virtual ~Graphics_Object();

    // Virtual member functions
    //   Render the module
    virtual void render() = 0;
    //   Execute on click
    virtual bool clicked();
    //   Update the location of this graphics object
    virtual void update_location(SDL_Rect);
    //   Set the color of this graphics object
    void set_color(SDL_Color);

    // Member functions
    //   Return true if the mouse is within the graphics object, false
    //   otherwise
    bool mouse_over();

protected:
    Graphics_Listener *listener;
};

#endif

