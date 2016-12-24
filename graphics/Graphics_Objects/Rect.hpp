/*
 * Matthew Diamond 2015
 * The Rect graphics object. This object is just a rectangle in some place in
 * the window, with some color. It also makes it possible to select input
 * modules by clicking.
 */

#ifndef GRAPHICS_RECT_HPP
#define GRAPHICS_RECT_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/*************************
 * RECT CLASS DEFINITION *
 *************************/

class Rect: public Graphics_Object
{
public:
    // Fill the rectangle or just draw the outline
    bool fill;

    // Constructor and destructor
    Rect(std::string, SDL_Rect, SDL_Color, Graphics_Listener *);
    virtual ~Rect();

    // Member functions
    virtual void render();
};

#endif

