cmake_minimum_required(VERSION 3.0)

project(Tuto-Lambda)

add_executable(display-obj 
    src/main.cpp 
    src/Texture.cpp
    src/SDLTexture.cpp
    src/draw.cpp
    src/objloader.cpp)

if (UNIX)
    target_link_libraries(display-obj PRIVATE m)
endif()

if (WIN32)
    # https://github.com/libsdl-org/SDL/releases/tag/release-2.24.2
    list(APPEND CMAKE_PREFIX_PATH "3rdparty/SDL2-2.24.2/cmake")
endif()

find_package(SDL2)

if (SDL2_FOUND)
    target_link_libraries(display-obj PRIVATE SDL2::SDL2)

    if (WIN32)
        target_link_libraries(display-obj PRIVATE SDL2::SDL2main)
        add_custom_command(
            TARGET display-obj POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:display-obj>
        )
    endif()

endif()

target_include_directories(display-obj PRIVATE src/)