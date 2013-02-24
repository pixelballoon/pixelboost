cmake_minimum_required (VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")

add_subdirectory (../shared ${CMAKE_BINARY_DIR}/shared)

set (PIPELINE_HEADERS
	"${PIPELINE_HEADERS}"
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/external"
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/shared"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	CACHE INTERNAL "Complete list of pipeline headers")

include_directories("${PIPELINE_HEADERS}")

set (DEBUG_SOURCES
	"src/pipeline/debug/log.h"
	)

set (FILE_SOURCES
	"src/pipeline/file/fileSystem.cpp"
	"src/pipeline/file/fileSystem.h"
	)

set (PIPELINE_SOURCES
	${DEBUG_SOURCES}
	${FILE_SOURCES}
	)

add_library (pixelboost_pipeline STATIC ${PIPELINE_SOURCES})
target_link_libraries (pixelboost_pipeline pixelboost_shared)

source_group (debug FILES ${DEBUG_SOURCES})
source_group (file FILES ${FILE_SOURCES})
