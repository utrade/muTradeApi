# Before running make, we need to run one of the following commands:
# cmake .. (equivalent to cmake -DCMAKE_BUILD_TYPE="Debug" ..)
# cmake -DCMAKE_BUILD_TYPE="Release" ..

cmake_minimum_required(VERSION 2.6)

project(fixClient)
set(fixClient_VERSION_MAJOR 1)
set(fixClient_VERSION_MINOR 0)
set(fixClient_VERSION_PATCH 0)

set(fixClient_VERSION_STRING 
  "${fixClient_VERSION_MAJOR}.${fixClient_VERSION_MINOR}.${fixClient_VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Setting Default Build Type : Debug ")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


find_package(Boost 1.42 REQUIRED COMPONENTS 
  python 
  system 
  filesystem
  serialization 
  thread
  program_options 
  regex 
  test_exec_monitor)

include_directories(${Boost_INCLUDE_DIRS})


find_package(QUICKFIX REQUIRED)
include_directories(${QUICKFIX_INCLUDE_DIRS})

#include_directories(${fixClient_SOURCE_DIR}/common)
#include_directories(${fixClient_SOURCE_DIR}/tbt)

set(INSTALL_TARGETS_DEFAULT RUNTIME DESTINATION .)

set(EXECUTABLE_OUTPUT_PATH ${fixClient_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${fixClient_SOURCE_DIR}/lib)

add_subdirectory(src)	



