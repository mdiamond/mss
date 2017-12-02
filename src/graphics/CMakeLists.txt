file(GLOB_RECURSE graphics_SRC
     "*.cpp"
     "*.hpp")

add_library(graphics ${graphics_SRC})

target_link_libraries(graphics ${SDL2_LIBRARIES})
target_link_libraries(graphics ${SDL2TTF_LIBRARIES})
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(graphics PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(graphics PUBLIC ${SDL2TTF_INCLUDE_DIRS})
target_compile_options(graphics PUBLIC ${SDL2_CFLAGS_OTHER})
target_compile_options(graphics PUBLIC ${SDL2TTF_CFLAGS_OTHER})

