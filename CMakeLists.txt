cmake_minimum_required(VERSION 2.8.12)

add_compile_options(-std=c++14)
add_compile_options(-g)
add_compile_options(-O0)
add_compile_options(-Wall)
add_compile_options(-pedantic)
add_compile_options(-fdiagnostics-color=always)
if(APPLE)
    add_compile_options(-stdlib=libc++)
endif()

project(mss)

file(GLOB mss_SRC
     "*.cpp"
     "*.hpp")

file(GLOB_RECURSE modules_SRC
     "modules/*.cpp"
     "modules/*.hpp")

add_executable(mss ${mss_SRC} ${modules_SRC})

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

add_subdirectory("graphics")

target_link_libraries(mss ${SDL2_LIBRARIES})
target_link_libraries(mss ${SDL2TTF_LIBRARIES})
target_link_libraries(mss graphics)
target_include_directories(mss PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(mss PUBLIC ${SDL2TTF_INCLUDE_DIRS})
target_include_directories(mss PUBLIC ${mss_SOURCE_DIR})
target_include_directories(mss PUBLIC ${mss_SOURCE_DIR}/modules)
target_compile_options(mss PUBLIC ${SDL2_CFLAGS_OTHER})
target_compile_options(mss PUBLIC ${SDL2TTF_CFLAGS_OTHER})

