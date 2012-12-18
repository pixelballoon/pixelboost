cmake_minimum_required (VERSION 2.8.10)
project (modeltool)

include (../../build.cmake)

pixelboost_setup_pre(../../ modeltool "com.pixelballoon.modeltool" "Modeltool")

include_directories("source")

set (GAME_SOURCES
	"osx/modelconvertor/modelconvertor/main.cpp"
	"source/modelexporter.cpp"
	"source/modelLoader.cpp"
	)

pixelboost_setup_post()
