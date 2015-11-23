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

#ifndef synth_graphics_object_h
#define synth_graphics_object_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/**********************************
 * TYPES OF GRAPHICS_OBJECTS ENUM *
 **********************************/

enum Graphics_Objects
{
    BUTTON = 0,
    INPUT_TEXT_BOX,
    INPUT_TOGGLE_BUTTON,
    PAGE,
    RECT,
    TEXT,
    TEXT_BOX,
    TOGGLE_BUTTON,
    WAVEFORM
};

/************************************
 * GRAPHICS_OBJECT CLASS DEFINITION *
 ************************************/

// Forward declaration of Module class
class Module;

class Graphics_Object
{
    public:
        // Module information
        std::string name;
        int type;
        Module *parent;
        SDL_Rect location;
        SDL_Color *color;
        bool updated;
        // Constructor and destructor
        Graphics_Object(std::string, int type, Module *,
                        SDL_Rect, SDL_Color *);
        virtual ~Graphics_Object();

        // Virtual member functions
        //   Render the module
        virtual void render() = 0;
        //   Execute on click
        virtual void clicked() = 0;

        // Member functions
        //   Return true if clicked, 0 otherwise
        bool was_clicked();
        //   Update this module's location
        void update_location(SDL_Rect);
        //   Update this module's alpha value
};

#endif
