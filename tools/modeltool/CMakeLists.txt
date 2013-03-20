cmake_minimum_required (VERSION 2.8.10)
project (modeltool)

include (FindFBX.cmake)

add_subdirectory (../../pipeline ${CMAKE_BINARY_DIR}/pipeline)

include_directories("${FBX_INCLUDE_DIR}")
include_directories("${PIPELINE_HEADERS}")
include_directories("source")

set (MODELTOOL_SOURCES
	"source/main.cpp"
	"source/modelExporter.cpp"
	"source/modelExporter.h"
	"source/modelLoader.cpp"
	"source/modelLoader.h"
	)

add_executable (modeltool ${MODELTOOL_SOURCES})
target_link_libraries (modeltool pixelboost_pipeline)

if (FBX_FOUND)
	target_link_libraries (modeltool ${FBX_LIBRARY})
endif (FBX_FOUND)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	target_link_libraries (modeltool
		"-framework Foundation"
		)
endif (PIXELBOOST_BUILD_PLATFORM_OSX)