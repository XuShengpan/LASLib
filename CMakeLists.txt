cmake_minimum_required(VERSION 3.0)
project(LASLib)

if(MSVC)
message("MSVC")
add_definitions(-D_USE_MATH_DEFINES)
endif()

#MPCDPS
set(MPCDPS_LIB_PATH ../MPCDPS/)
include_directories(${MPCDPS_LIB_PATH}/Core/include/)
link_directories(${MPCDPS_LIB_PATH}/lib/)

include_directories(./include)

file(GLOB INC_LIST "./include/*.h")
file(GLOB SRC_LIST "./src/*.cpp")

set(LIBRARY_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/lib/)

add_library(LASlib 
${INC_LIST}
${SRC_LIST}
)

target_link_libraries(LASlib mpcdps_core)

add_subdirectory(test)

