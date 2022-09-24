project(Tuto-Lambda)

add_executable(gen_image 
    src/main.cpp 
    src/bitmap.cpp
    src/draw.cpp
    src/objloader.cpp)

target_link_libraries(gen_image PRIVATE m)
target_include_directories(gen_image PRIVATE src/)