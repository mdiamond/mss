/*
 * Matthew Diamond 2015
 * The Rect graphics object. This object is just a rectangle
 * in some place in the window, with some color.
 */

#ifndef synth_rect_h
#define synth_rect_h

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
        Rect(std::string, SDL_Rect, SDL_Color, Module *);
        virtual ~Rect();
        // Member functions
        virtual void render();
        virtual void clicked();
};

#endif
