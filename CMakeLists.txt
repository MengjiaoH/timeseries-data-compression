cmake_minimum_required(VERSION 3.11)
project(DifferentialVolumeRendering)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option (BUILD_VTK "Build VTK Analysis" ON)
option (BUILD_PIDX "Build PIDX Analysis" OFF)
option (BUILD_RAW "Build RAW Analysis" OFF)

## Find ospcommon
find_package(ospcommon REQUIRED)

#build utils
add_subdirectory("${CMAKE_SOURCE_DIR}/utils")
include_directories("${CMAKE_SOURCE_DIR}/utils")


if(BUILD_VTK)
    add_subdirectory(vtk)
endif()

if(BUILD_PIDX)
    add_subdirectory(pidx)
endif()

if(BUILD_RAW)
    add_subdirectory(raw)
endif()

