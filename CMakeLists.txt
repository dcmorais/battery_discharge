cmake_minimum_required(VERSION 3.13)
project(battery_discharge)

set(CMAKE_CXX_STANDARD 14)

include_directories(RS-232)
add_subdirectory(RS-232)
include_directories(ElectronicLoad)
add_subdirectory(ElectronicLoad)

add_executable(battery_discharge main.cpp)
target_link_libraries(ElectronicLoad RS-232)
target_link_libraries(battery_discharge ElectronicLoad)
