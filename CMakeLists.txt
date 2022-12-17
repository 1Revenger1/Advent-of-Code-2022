cmake_minimum_required(VERSION 3.0.0)
project(Advent-of-Code-2022 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
include(CTest)
enable_testing()

SET(DAY "17")
FILE(GLOB DayFiles "src/Day ${DAY}/*.cpp")
add_executable(Advent-of-Code-2022 ${DayFiles})
target_include_directories(Advent-of-Code-2022 PRIVATE include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
