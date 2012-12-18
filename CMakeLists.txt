cmake_minimum_required (VERSION 2.8.10)
project (shadertool)

include (../../build.cmake)

pixelboost_setup_pre(../../ shadertool "com.pixelballoon.shadertool" "Shadertool")

include_directories("source")

set (GAME_SOURCES
	"source/main.cpp"
	)

pixelboost_setup_post()
