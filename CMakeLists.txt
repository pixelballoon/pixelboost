cmake_minimum_required (VERSION 2.8.10)
project (texturetool)

include (../../build.cmake)

pixelboost_setup_pre(../../ texturetool "com.pixelballoon.texturetool" "Texturetool")

include_directories("source")

set (GAME_SOURCES
	"source/main.cpp"
	)

pixelboost_setup_post()
