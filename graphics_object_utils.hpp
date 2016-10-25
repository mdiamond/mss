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

#endif

