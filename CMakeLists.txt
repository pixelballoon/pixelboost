cmake_minimum_required (VERSION 2.8.10)
project (modeltool)

include (../../build.cmake)
include (FindFBX.cmake)

pixelboost_setup_pre(../../ modeltool "com.pixelballoon.modeltool" "Modeltool")

include_directories(${FBX_INCLUDE_DIR})
include_directories("source")

set (GAME_SOURCES
	"source/main.cpp"
	"source/modelexporter.cpp"
	"source/modelLoader.cpp"
	)

pixelboost_setup_post()

if (FBX_FOUND)
	target_link_libraries (modeltool ${FBX_LIBRARY})
endif (FBX_FOUND)
