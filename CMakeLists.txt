# Name the project you can choose any name you want here
PROJECT(motr) 

# Check requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

# set the compiler flags
SET(CMAKE_BUILD_TYPE debug)
SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Add path to additional packages (makes it easier to include common libraries)
set(CMAKE_MODULE_PATH ${motr_SOURCE_DIR}/CMakeModules/)

# Define where your executables should be put
set(EXECUTABLE_OUTPUT_PATH ${motr_BINARY_DIR})

IF(WIN32)
#ADD_SUBDIRECTORY(vendor/gtest-1.7.0)
ADD_SUBDIRECTORY(vendor/gmock-1.7.0)
ENDIF()

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(motr)


