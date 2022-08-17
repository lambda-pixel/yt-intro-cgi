project(Tuto-Lambda)

add_executable(gen_image main.cpp bitmap.cpp)

target_link_libraries(gen_image PRIVATE m)