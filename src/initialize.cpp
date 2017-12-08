/*
 * Matthew Diamond 2016
 * All initialization related functions.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

// Included SDL components
#include "SDL.h"

// Included files
#include "graphics_config.hpp"
#include "graphics_object_utils.hpp"
#include "initialize.hpp"
#include "main.hpp"
#include "populate_wavetables.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Modules/Output.hpp"

// Included "other" classes
#include "Color_Modifier.hpp"

/********************************
 * MAIN INITIALIZATION FUNCTION *
 ********************************/

/*
 * Initialize all of the stuff that needs to be initialized before audio can be
 * processed. Return true if all of this succeeds, false if anything fails.
 */
bool initialize()
{
    TTF_Font *font = nullptr;

    // Initialize SDL with the video and audio subsystems
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1))
    {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL initialized" << std::endl;

    // Initialize audio output device
    if(!open_audio_output_device())
    {
        return false;
    }

    // Initialize audio input device
    if(!open_audio_input_device())
    {
        return false;
    }

    // Initialize truetype
    if(TTF_Init() == -1)
    {
        std::cout << "Could not initialize TTF: " << TTF_GetError() << std::endl;
        return false;
    }

    // Open ttf font
    if(!load_font(&font))
    {
        return false;
    }

    // Initialize graphics library
    initialize_graphics_library(font);
    std::cout << "Graphics library initialized" << std::endl;

    // Open a window
    if(!open_window())
    {
        return false;
    }

    // Create a renderer
    if(!create_renderer())
    {
        return false;
    }

    // Initialize utilities sub page
    initialize_utilities_page();

    // Create a texture to render to
    if(!create_texture())
    {
        return false;
    }

    // Populate wavetables
    populate_wavetables();

    // Seed rand()
    srand(time(NULL));

    // Initialize the output module
    initialize_output();

    // Set colors on the utilities page graphics objects
    prettify_utilities_page();

    // Unpause the audio
    SDL_PauseAudioDevice(AUDIO_INPUT_DEVICE_ID, 0);
    SDL_PauseAudio(0);
    std::cout << "Audio unpaused" << std::endl;

    return true;
}

/********************************************
 * GRAPHICS RELATED INITIALIZATION FUNCTION *
 ********************************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
bool open_window()
{
    Uint32 window_flags = SDL_WINDOW_ALLOW_HIGHDPI;

    WINDOW = SDL_CreateWindow("mss",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              window_flags);

    if(WINDOW == NULL)
    {
        std::cout << RED_STDOUT << "Could not create window: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Window opened" << std::endl;

    return true;
}

/*
 * Create a renderer for the window created before.
 */
bool create_renderer()
{
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
                          SDL_RENDERER_PRESENTVSYNC;

    RENDERER = SDL_CreateRenderer(WINDOW, -1, render_flags);

    if(WINDOW == NULL)
    {
        std::cout << RED_STDOUT << "Could not create renderer: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Renderer created" << std::endl;

    return true;
}

/*
 * Create a texture to render to so that the GPU will be used
 * isntead of the CPU when rendering to a surface.
 */
bool create_texture()
{
    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET,
                                WINDOW_WIDTH, WINDOW_HEIGHT);

    if(TEXTURE == NULL)
    {
        std::cout << RED_STDOUT << "Could not create texture: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Texture created" << std::endl;

    SDL_SetRenderTarget(RENDERER, TEXTURE);

    return true;
}

/*
 * Load font into external variables so that all
 * graphics objects can render text.
 */
bool load_font(TTF_Font **font)
{
    *font = TTF_OpenFont("../fonts/visitor1.ttf", 10);

    if(!(*font))
    {
        std::cout << RED_STDOUT << "Could not open the TTF font: "
                  << TTF_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Font loaded" << std::endl;

    return true;
}

/*
 * Create the very first sub page on every page, the utilities sub page.
 * This sub page contains the buttons for adding new modules, switching
 * pages, and saving or loading the current patch.
 */
void initialize_utilities_page()
{
    SDL_Rect location;
    std::map<std::string, SDL_Rect> locations;
    std::map<std::string, Graphics_Object *> graphics_objects;
    std::vector<Graphics_Object *> graphics_objects_vector;

    location =
        {2, WINDOW_HEIGHT - (MENU_HEIGHT * 2), 49, 9};
    locations["add adsr button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 55,
         location.h};
    locations["add delay button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 61,
         location.h};
    locations["add filter button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 55,
         location.h};
    locations["add input button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 57,
         location.h};
    locations["add mixer button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 85,
         location.h};
    locations["add multiplier button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 55,
         location.h};
    locations["add noise button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 85,
         location.h};
    locations["add oscillator button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 43,
         location.h};
    locations["add sah button"] = location;
    location =
        {WINDOW_WIDTH - 79 - 55 - (2 * MODULE_SPACING), location.y, 79,
         location.h};
    locations["previous page button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, 55,
         location.h};
    locations["next page button"] = location;
    location =
        {2, location.y + 9 + MODULE_SPACING,
         (WINDOW_WIDTH / 2) - MODULE_SPACING - 1, location.h};
    locations["save patch button"] = location;
    location =
        {location.x + location.w + MODULE_SPACING, location.y, location.w,
         location.h};
    locations["load patch button"] = location;

    // Create the background and add it to the map of graphics objects
    graphics_objects["background rect"] =
        new Rect("background rect", WINDOW_RECT, BLACK, NULL);

    // Create the button objects and add them to the map of graphics objects
    graphics_objects["add adsr button"] =
        new Button("add adsr button", locations["add adsr button"], WHITE,
                   BLACK, "ADD ADSR", NO_MODULE_LISTENER);
    graphics_objects["add delay button"] =
        new Button("add delay button", locations["add delay button"], WHITE,
                   BLACK, "ADD DELAY", NO_MODULE_LISTENER);
    graphics_objects["add filter button"] =
        new Button("add filter button", locations["add filter button"], WHITE,
                   BLACK, "ADD FILTER", NO_MODULE_LISTENER);
    graphics_objects["add input button"] =
        new Button("add input button", locations["add input button"], WHITE,
                   BLACK, "ADD INPUT", NO_MODULE_LISTENER);
    graphics_objects["add mixer button"] =
        new Button("add mixer button", locations["add mixer button"], WHITE,
                   BLACK, "ADD MIXER", NO_MODULE_LISTENER);
    graphics_objects["add multiplier button"] =
        new Button("add multiplier button", locations["add multiplier button"],
                   WHITE, BLACK, "ADD MULTIPLIER", NO_MODULE_LISTENER);
    graphics_objects["add noise button"] =
        new Button("add noise button", locations["add noise button"], WHITE,
                   BLACK, "ADD NOISE", NO_MODULE_LISTENER);
    graphics_objects["add oscillator button"] =
        new Button("add oscillator button", locations["add oscillator button"],
                   WHITE, BLACK, "ADD OSCILLATOR", NO_MODULE_LISTENER);
    graphics_objects["add sah button"] =
        new Button("add sah button", locations["add sah button"], WHITE, BLACK,
                   "ADD SAH", NO_MODULE_LISTENER);
    graphics_objects["previous page button"] =
        new Button("previous page button", locations["previous page button"],
                   WHITE, BLACK, "PREVIOUS PAGE", NO_MODULE_LISTENER);
    graphics_objects["next page button"] =
        new Button("next page button", locations["next page button"], WHITE,
                   BLACK, "NEXT PAGE", NO_MODULE_LISTENER);
    graphics_objects["save patch button"] =
        new Button("save patch button", locations["save patch button"], WHITE,
                   BLACK, "SAVE PATCH", NO_MODULE_LISTENER);
    graphics_objects["load patch button"] =
        new Button("load patch button", locations["load patch button"], WHITE,
                   BLACK, "LOAD PATCH", NO_MODULE_LISTENER);

    graphics_objects_vector.push_back(graphics_objects["background rect"]);
    for(auto itr = graphics_objects.begin(); itr != graphics_objects.end();
        itr ++)
    {
        if(itr->first != "background rect")
        {
            graphics_objects_vector.push_back(itr->second);
        }
    }

    // Create the utilities page
    UTILITIES_PAGE = new Page("utilities & background page", WINDOW_RECT, BLACK,
                              &graphics_objects_vector);

    std::cout << "Utilities page initialized" << std::endl;
}

/*
 * Set the graphics objects on the utilities page to the same color as
 * the output module.
 */
void prettify_utilities_page()
{
    for(auto itr = UTILITIES_PAGE->graphics_objects.begin();
        itr != UTILITIES_PAGE->graphics_objects.end(); itr ++)
    {
        if((*itr)->graphics_object_type
           == Graphics_Object::BUTTON)
        {
            ((Button *) (*itr))->set_colors(
                MODULES[0]->primary_module_color, MODULES[0]->secondary_module_color);
        }
        else if((*itr)->graphics_object_type
                == Graphics_Object::TEXT_BOX)
        {
            ((Text_Box *) (*itr))->set_colors(
                MODULES[0]->primary_module_color, MODULES[0]->secondary_module_color);
        }
    }
}

/*****************************************
 * AUDIO RELATED INITIALIZATION FUNCTION *
 *****************************************/

/*
 * Open the audio output device with a simple configuration. Return whether or
 * not opening the audio output device was successful.
 */
bool open_audio_output_device()
{
    SDL_AudioSpec wanted, obtained;

    wanted.freq = SAMPLE_RATE;
    wanted.format = AUDIO_F32;
    wanted.channels = 2;
    wanted.samples = 1024;
    wanted.callback = audio_output_callback;
    wanted.userdata = NULL;

    if(SDL_OpenAudio(&wanted, &obtained) < 0)
    {
        std::cout << RED_STDOUT << "Could not open the audio output device: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Audio output device opened" << std::endl;
    std::cout << "    Sample rate: " << obtained.freq << std::endl;
    std::cout << "    Format: " << obtained.format << std::endl;
    std::cout << "    Channels: " << obtained.channels << std::endl;
    std::cout << "    Buffer size in samples: " << obtained.samples << std::endl;
    std::cout << "    Buffer size in bytes: " << obtained.size << std::endl;

    BUFFER_SIZE = obtained.samples;
    NUM_CHANNELS = obtained.channels;

    // Return success
    return true;
}

/*
 * Open the audio input device. Return whether or not it was opened
 * successfully.
 */
bool open_audio_input_device()
{
    SDL_AudioSpec wanted, obtained;

    wanted.freq = SAMPLE_RATE;
    wanted.format = AUDIO_F32;
    wanted.channels = 1;
    wanted.samples = 1024;
    wanted.callback = audio_input_callback;
    wanted.userdata = NULL;

    /* const int count = SDL_GetNumAudioDevices(1); */

    /* for(int i = 0; i < count; i ++) */
    /* { */
    /*     std::cout << "Device #" << i << ", device name: " */
    /*               << SDL_GetAudioDeviceName(i, 1) << std::endl; */
    /* } */

    AUDIO_INPUT_DEVICE_ID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 1),
                                                1, &wanted, &obtained, 0);

    if(AUDIO_INPUT_DEVICE_ID == 0)
    {
        std::cout << RED_STDOUT << "Could not open the audio input device: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return false;
    }
    if(obtained.samples != BUFFER_SIZE)
    {
        std::cout << RED_STDOUT << "Audio input buffer is not the same size "
                                   "as audio output buffer!"
                                << DEFAULT_STDOUT << std::endl;
        return false;
    }

    std::cout << "Audio input device opened" << std::endl;
    std::cout << "    Sample rate: " << obtained.freq << std::endl;
    std::cout << "    Format: " << obtained.format << std::endl;
    std::cout << "    Channels: " << obtained.channels << std::endl;
    std::cout << "    Buffer size in samples: " << obtained.samples << std::endl;
    std::cout << "    Buffer size in bytes: " << obtained.size << std::endl;

    AUDIO_IN = std::vector<float>(BUFFER_SIZE, 0);

    // Return success
    return true;
}

/*
 * Initialize the one and only output module.
 */
void initialize_output()
{
    // Create the output module
    Output *output = new Output();
    output->initialize_graphics_objects();
    MODULES.push_back(output);

    std::cout << "Output initialized" << std::endl;
}

