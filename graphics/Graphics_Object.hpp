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

#ifndef MSS_GRAPHICS_OBJECT_HPP
#define MSS_GRAPHICS_OBJECT_HPP

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

// Forward declaration of module and graphics listener classes
class Module;
class Graphics_Listener;

class Graphics_Object
{
public:
    // Graphics_Object type enum
    enum GraphicsObjectType
    {
        BUTTON = 0,
        INPUT_TEXT_BOX,
        INPUT_TOGGLE_BUTTON,
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
    Module *parent;
    SDL_Rect location;
    SDL_Color color;
    bool updated;
    // Constructor and destructor
    Graphics_Object(std::string, GraphicsObjectType graphics_object_type,
                    Module *, SDL_Rect, SDL_Color);
    virtual ~Graphics_Object();

    // Virtual member functions
    //   Render the module
    virtual void render() = 0;
    //   Execute on click
    virtual void clicked() = 0;
    //   Update the location of this graphics object
    virtual void update_location(SDL_Rect);
    //   Set the color of this graphics object
    void set_color(SDL_Color);

    // Member functions
    //   Return true if clicked, false otherwise
    bool was_clicked();

protected:
    Graphics_Listener *listener;
};

#endif

