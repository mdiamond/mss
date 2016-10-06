cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall -fdiagnostics-color=always")

project(synth)

set(TOP_SOURCES
    event_handler.cpp
    function_forwarder.cpp
    Graphics_Object.cpp
    graphics_object_utils.cpp
    image_processing.cpp
    load_patch.cpp
    main.cpp
    main_helpers.cpp
    Module.cpp
    module_utils.cpp
    populate_wavetables.cpp
    save_patch.cpp
    signal_processing.cpp
    tests.cpp
    Timer.cpp
)

set(MODULE_SOURCES
    Modules/Adsr.cpp
    Modules/Delay.cpp
    Modules/Filter.cpp
    Modules/Mixer.cpp
    Modules/Multiplier.cpp
    Modules/Noise.cpp
    Modules/Oscillator.cpp
    Modules/Output.cpp
    Modules/Sah.cpp
)

set(GRAPHICS_OBJECT_SOURCES
    Graphics_Objects/Button.cpp
    Graphics_Objects/Input_Text_Box.cpp
    Graphics_Objects/Input_Toggle_Button.cpp
    Graphics_Objects/Page.cpp
    Graphics_Objects/Rect.cpp
    Graphics_Objects/Text_Box.cpp
    Graphics_Objects/Text.cpp
    Graphics_Objects/Toggle_Button.cpp
    Graphics_Objects/Waveform.cpp
)

add_executable(synth ${TOP_SOURCES} ${MODULE_SOURCES} ${GRAPHICS_OBJECT_SOURCES})

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
message(STATUS "SDL2 include directory: " ${SDL2_INCLUDE_DIRS})
message(STATUS "SDL2 library directory: " ${SDL2_LIBRARIES})
message(STATUS "SDL2 cflags other: " ${SDL2_CFLAGS_OTHER})
message(STATUS "SDL2_ttf include directory: " ${SDL2TTF_INCLUDE_DIRS})
message(STATUS "SDL2_ttf library directory: " ${SDL2TTF_LIBRARIES})
message(STATUS "SDL2_ttf cflags other: " ${SDL2TTF_CFLAGS_OTHER})

target_link_libraries(synth ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
target_include_directories(synth PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
target_compile_options(synth PUBLIC ${SDL2_CFLAGS_OTHER} ${SDL2TTF_CFLAGS_OTHER})
