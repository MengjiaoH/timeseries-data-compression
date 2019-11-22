## Find MPI
find_package(MPI REQUIRED)

## Find PIDX
find_package(PIDX REQUIRED)

include_directories(${MPI_CXX_INCLUDE_PATH} ${PIDX_INCLUDE_DIRS})
target_link_libraries(libs ospcommon::ospcommon ${PIDX_LIBRARIES} ${MPI_CXX_LIBRARIES} util)

add_executable(pidx-analysis pidxMain.cpp)
set_target_properties(pidx-analysis PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}")

add_library(pidx_libs SHARED    pidxUtils.h
                                loadPIDX.h
                                loadPIDX.cpp)

target_link_libraries(pidx_libs libs)

target_link_libraries(pidx-analysis pidx_libs)   


