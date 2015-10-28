/*
 * Matthew Diamond 2015
 * Functions for doing batch operations on graphics objects.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"

using namespace std;

/**********************************
 * BATCH GRAPHICS OBJECT CREATORS *
 **********************************/

vector<Text *> create_text_objects(vector<string> *names, vector<SDL_Rect> *locations, vector<SDL_Color> *colors,
           vector<string> *texts, vector<TTF_Font *> *fonts)
{
    
}