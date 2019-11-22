find_package(zfp REQUIRED)

INCLUDE_DIRECTORIES(${ZFP_INCLUDE_DIR})

add_library(util
    helper.h
    parseArgs.h
    parseArgs.cpp
    writeFile.h)

set_target_properties(util PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON)

target_include_directories(util PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_link_libraries(util PUBLIC ospcommon::ospcommon ${ZFP_LIBRARIES})