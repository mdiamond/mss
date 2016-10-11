cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall -fdiagnostics-color=always")

project(mss)

set(TOP_SOURCES
    event_handler.cpp
    function_forwarder.cpp
    graphics_object_utils.cpp
    image_processing.cpp
    initialize.cpp
    load_patch.cpp
    main.cpp
    main_helpers.cpp
    module_utils.cpp
    populate_wavetables.cpp
    save_patch.cpp
    signal_processing.cpp
    tests.cpp
    Timer.cpp
)

set(MODULE_SOURCES
    modules/Module.cpp
    modules/Modules/Adsr.cpp
    modules/Modules/Delay.cpp
    modules/Modules/Filter.cpp
    modules/Modules/Mixer.cpp
    modules/Modules/Multiplier.cpp
    modules/Modules/Noise.cpp
    modules/Modules/Oscillator.cpp
    modules/Modules/Output.cpp
    modules/Modules/Sah.cpp
)

set(GRAPHICS_OBJECT_SOURCES
    graphics/Graphics_Object.cpp
    graphics/Graphics_Objects/Button.cpp
    graphics/Graphics_Objects/Input_Text_Box.cpp
    graphics/Graphics_Objects/Input_Toggle_Button.cpp
    graphics/Graphics_Objects/Page.cpp
    graphics/Graphics_Objects/Rect.cpp
    graphics/Graphics_Objects/Text_Box.cpp
    graphics/Graphics_Objects/Text.cpp
    graphics/Graphics_Objects/Toggle_Button.cpp
    graphics/Graphics_Objects/Waveform.cpp
)

add_executable(mss ${TOP_SOURCES} ${MODULE_SOURCES} ${GRAPHICS_OBJECT_SOURCES})

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
message(STATUS "Root level project directory: " ${mss_SOURCE_DIR})
message(STATUS "SDL2 include directory: " ${SDL2_INCLUDE_DIRS})
message(STATUS "SDL2 library directory: " ${SDL2_LIBRARIES})
message(STATUS "SDL2 cflags other: " ${SDL2_CFLAGS_OTHER})
message(STATUS "SDL2_ttf include directory: " ${SDL2TTF_INCLUDE_DIRS})
message(STATUS "SDL2_ttf library directory: " ${SDL2TTF_LIBRARIES})
message(STATUS "SDL2_ttf cflags other: " ${SDL2TTF_CFLAGS_OTHER})

target_link_libraries(mss ${SDL2_LIBRARIES})
target_link_libraries(mss ${SDL2TTF_LIBRARIES})
target_include_directories(mss PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(mss PUBLIC ${SDL2TTF_INCLUDE_DIRS})
target_include_directories(mss PUBLIC ${mss_SOURCE_DIR})
target_include_directories(mss PUBLIC ${mss_SOURCE_DIR}/graphics)
target_include_directories(mss PUBLIC ${mss_SOURCE_DIR}/modules)
target_compile_options(mss PUBLIC ${SDL2_CFLAGS_OTHER})
target_compile_options(mss PUBLIC ${SDL2TTF_CFLAGS_OTHER})

