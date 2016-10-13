/*
 * Matthew Diamond 2015
 * Header file for the functions for constructing batches of graphics objects.
 */

#ifndef MSS_GRAPHICS_OBJECT_UTILS_HPP
#define MSS_GRAPHICS_OBJECT_UTILS_HPP

/************
 * INCLUDES *
 ************/

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// A function to increment or decrement the current page number
void increment_page_number(int);

// Batch graphics object constructors
std::vector<Graphics_Object *> initialize_button_objects(
    std::vector<std::string>, std::vector<SDL_Rect>,
    std::vector<SDL_Color *>, std::vector<SDL_Color *>,
    std::vector<std::string>, std::vector<Module *>);
std::vector<Graphics_Object *> initialize_text_box_objects(
    std::vector<std::string>, std::vector<SDL_Rect>,
    std::vector<SDL_Color *>, std::vector<SDL_Color *>,
    std::vector<std::string>, std::vector<TTF_Font *>,
    std::vector<Module *>);
std::vector<Graphics_Object *> initialize_text_objects(
    std::vector<std::string>, std::vector<SDL_Rect>,
    std::vector<SDL_Color *>, std::vector<std::string>,
    std::vector<TTF_Font *>);
std::vector<Graphics_Object *> initialize_toggle_button_objects(
    std::vector<std::string>, std::vector<SDL_Rect>, std::vector<SDL_Color *>,
    std::vector<SDL_Color *>, std::vector<SDL_Color *>, std::vector<SDL_Color *>,
    std::vector<TTF_Font *>,
    std::vector<std::string>, std::vector<std::string>,
    std::vector<bool>, std::vector<Module *>);
std::vector<Graphics_Object *> initialize_waveform_objects(
    std::vector<std::string>, std::vector<SDL_Rect>,
    std::vector<SDL_Color *>, std::vector<SDL_Color *>, std::vector<float>,
    std::vector<float>, std::vector<std::vector<float> *>);

#endif

