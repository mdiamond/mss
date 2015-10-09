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

// Included classes
#include "Text.hpp"

/*****************************
 * TEXT BOX CLASS DEFINITION *
 *****************************/

class Text_Box: public Graphics_Object
{
    public:
        TTF_Font *font;
        Text *text;
        Text *prompt_text;
        Text *typing_text;
        bool active;
        // Constructor and destructor
        Text_Box(std::string *, SDL_Rect *, SDL_Color *,
                 std::string *, std::string *, std::string *,
                 TTF_Font *, Module *);
        virtual ~Text_Box();
        // Member functions
        virtual void render_graphics_object();
        virtual void clicked();
        void typed(char *);
        void delete_character();
        void entered();
};

#endif
