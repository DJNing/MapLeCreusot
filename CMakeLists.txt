cmake_minimum_required(VERSION 3.14)

# Set the project name and version
project(MapLeCreusot VERSION 1.0)

# Specify the required Qt modules
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Set C++11 as the standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler definitions
add_definitions(-DQT_DEPRECATED_WARNINGS)
# Uncomment this line if you want to disable deprecated APIs before Qt 6
# add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# Find libosmium
find_path(LIBOSMIUM_INCLUDE_DIRS "osmium/osm.hpp")
if(NOT LIBOSMIUM_INCLUDE_DIRS)
    message(FATAL_ERROR "libosmium not found. Please install libosmium.")
endif()

# Find protozero
find_path(PROTOZERO_INCLUDE_DIRS "protozero/byteswap.hpp")
if(NOT PROTOZERO_INCLUDE_DIRS)
    message(FATAL_ERROR "protozero not found. Please install protozero.")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LIBOSMIUM_INCLUDE_DIRS}
    ${PROTOZERO_INCLUDE_DIRS}
)

# Define sources (now located in the src folder)
file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Define UI forms
set(FORMS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/mainwindow.ui
)

# Tell CMake where to look for the .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)  # set the folder where .ui files are

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${FORMS})

# Link libraries
target_link_libraries(
    ${PROJECT_NAME} 
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets 
    pthread 
    expat
    z
)

# Install target
install(
    TARGETS 
        ${PROJECT_NAME} 
    DESTINATION 
        /opt/${PROJECT_NAME}/bin
)