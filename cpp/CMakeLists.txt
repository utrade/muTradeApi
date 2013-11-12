cmake_minimum_required(VERSION 2.6)

project(futFutTest)
set(futFutTest_VERSION_MAJOR 0)
set(futFutTest_VERSION_MINOR 0)
set(futFutTest_VERSION_PATCH 1)

set(futFutTest_VERSION_STRING
  "${futFutTest_VERSION_MAJOR}.${futFutTest_VERSION_MINOR}.${futFutTest_VERSION_PATCH}"
  )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Setting Default Build Type : Debug")
endif(NOT CMAKE_BUILD_TYPE)

#FIND_PACKAGE(Subversion)
#IF(Subversion_FOUND)
#  Subversion_WC_INFO(${CMAKE_HOME_DIRECTORY} futFutTest)
#  MESSAGE("Current revision is ${futFutTest_WC_REVISION}")
#ENDIF(Subversion_FOUND)

configure_file (
  "${futFutTest_SOURCE_DIR}/config.h.in"
  "${futFutTest_SOURCE_DIR}/config.h"
  )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Boost 1.42 REQUIRED COMPONENTS
  system
  filesystem
  thread
  program_options
  regex
  )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall  -lrt")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -lrt")

include_directories(${futFutTest_SOURCE_DIR}/include)
#include_directories(${futFutTest_SOURCE_DIR}/src)

SET(EXECUTABLE_OUTPUT_PATH ${futFutTest_SOURCE_DIR}/bin) 
SET(LIBRARY_OUTPUT_PATH ${futFutTest_SOURCE_DIR}/bin) 

add_subdirectory(include)
add_subdirectory(src)
