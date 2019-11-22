find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}")

add_executable(vtk-analysis VTKMain.cpp)

set_target_properties(vtk-analysis PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

add_library(vtk_libs loadVTK.h
                     loadVTK.cpp)       


target_link_libraries(vtk_libs ${VTK_LIBRARIES} ospcommon::ospcommon)

target_link_libraries(vtk-analysis vtk_libs util)     
