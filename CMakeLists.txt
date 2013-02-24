cmake_minimum_required (VERSION 2.8.10)
project (shadertool)

add_subdirectory (../../pipeline ${CMAKE_BINARY_DIR}/pipeline)

include_directories(${PIPELINE_HEADERS})
include_directories("source")

set (SHADERTOOL_SOURCES
	"source/main.cpp"
	)

add_executable (shadertool ${SHADERTOOL_SOURCES})
target_link_libraries (shadertool pixelboost_pipeline)
