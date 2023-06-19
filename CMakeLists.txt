cmake_minimum_required(VERSION 3.20)
project(libSuperCpp4Win)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
#add_definitions("-DNOT_CHECK_OUT_RANGE=false")

add_executable(libSuperCpp4Win enhance/array/Array.h run.cpp)


target_link_libraries(libSuperCpp4Win fmt::fmt)
target_link_libraries(libSuperCpp4Win nlohmann_json::nlohmann_json)
