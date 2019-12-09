include_directories("${CMAKE_SOURCE_DIR}")

add_executable(raw-analysis RawMain.cpp)

set_target_properties(raw-analysis PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

add_library(raw_libs loadRaw.h
                     loadRaw.cpp)

target_link_libraries(raw_libs ospcommon::ospcommon)

target_link_libraries(raw-analysis raw_libs util)
