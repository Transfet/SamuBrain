project(SamuBrain)

cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

add_definitions(-DGAMEOFLIFE)
add_definitions(-DQ_LOOKUP_TABLE)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ${OpenMP_CXX_FLAGS}")

set(SamuMovie_SRCS main.cpp SamuLife.cpp GameOfLife.cpp VideoConverter.cpp SamuBrain.cpp)

add_executable(SamuBrain ${SamuMovie_SRCS})

target_link_libraries(SamuBrain Qt5::Core Qt5::Widgets ${OpenCV_LIBS})
