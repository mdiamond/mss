/*
 * Matthew Diamond 2015
 * The Text Box graphics object. This object allows the user
 * to type into it. The data entered can be used to change variables.
 */

#ifndef synth_text_box_h
#define synth_text_box_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*****************************
 * TEXT BOX CLASS DEFINITION *
 *****************************/

class Text_Box: public Graphics_Object
{
    public:
        TTF_Font *font;
        std::string text;
        std::string old_text;
        SDL_Texture *texture;
        bool active;
        // Constructor and destructor
        Text_Box(std::string *, SDL_Rect *, SDL_Color *, std::string *, TTF_Font *);
        virtual ~Text_Box();
        // Member functions
        virtual void render_graphics_object();
        virtual void clicked();
};

#endif
