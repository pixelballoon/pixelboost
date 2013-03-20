cmake_minimum_required (VERSION 2.8.10)
project (texturetool)

add_subdirectory (../../pipeline ${CMAKE_BINARY_DIR}/pipeline)

include_directories("${PIPELINE_HEADERS}")
include_directories("source")

set (TEXTURETOOL_SOURCES
	"source/main.cpp"
	)

add_executable (texturetool ${TEXTURETOOL_SOURCES})
target_link_libraries (texturetool pixelboost_pipeline)