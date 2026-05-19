cmake_minimum_required(VERSION 3.11)
project(Starva)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FIT_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/fit-cpp-sdk-21.202.0)
set(NLOHMANN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/nlohmann)
set(FIT_SDK_SRC_DIR ${FIT_SDK_DIR}/src)

include_directories(${FIT_SDK_SRC_DIR})
include_directories(include)
include_directories(vendor)

file(GLOB FIT_SDK_SOURCES "${FIT_SDK_SRC_DIR}/*.cpp")

set(SOURCES
        src/main.cpp
        src/RecordListener.cpp
        include/recordListener.hpp
        include/routePoint.hpp
        include/fitParser.hpp
        src/fitParser.cpp
        include/activity.hpp
        src/activity.cpp
        include/mapGenerator.hpp
        src/mapGenerator.cpp
)

add_executable(starva_app ${SOURCES} ${FIT_SDK_SOURCES})

target_include_directories(starva_app PRIVATE
        ${FIT_SDK_SRC_DIR}
        include
        vendor
)
