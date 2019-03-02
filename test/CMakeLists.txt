
link_directories(../lib/)

add_executable(las_lib_test main.cpp)

target_link_libraries(las_lib_test LASlib mpcdps_core)