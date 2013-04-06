cmake_minimum_required (VERSION 2.8.10)

set (PHYSFS_ARCHIVE_7Z FALSE CACHE BOOL "Enable 7zip support")
set (PHYSFS_BUILD_TEST FALSE CACHE BOOL "Build stdio test program.")

if (PIXELBOOST_LIBRARY_TYPE STREQUAL "STATIC")
	set (PHYSFS_BUILD_SHARED FALSE CACHE BOOL "Build physfs shared library")
	set (BOX2D_BUILD_SHARED FALSE CACHE BOOL "Build Box2D shared libraries")

	add_subdirectory (libs/physfs)
	pixelboost_setup_library(physfs-static)

	add_subdirectory (libs/box2d)
	pixelboost_setup_library(Box2D)
else (PIXELBOOST_LIBRARY_TYPE STREQUAL "STATIC")
	set (PHYSFS_BUILD_STATIC FALSE CACHE BOOL "Build physfs static library")
	set (BOX2D_BUILD_STATIC FALSE CACHE BOOL "Build Box2D static libraries")

	add_subdirectory (libs/physfs)
	pixelboost_setup_library(physfs-shared)

	add_subdirectory (libs/box2d)
	pixelboost_setup_library(Box2D_shared)
endif (PIXELBOOST_LIBRARY_TYPE STREQUAL "STATIC")

add_subdirectory (libs/freetype2)
pixelboost_setup_library(freetype2)

add_subdirectory (libs/lua)
pixelboost_setup_library(lua)

add_subdirectory (libs/enet)
pixelboost_setup_library(enet)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	add_subdirectory (libs/ddhidlib)
endif (PIXELBOOST_BUILD_PLATFORM_OSX)

if (NOT TARGET pixelboost_shared)
	add_subdirectory (shared)
	pixelboost_setup_library(pixelboost_shared)
endif (NOT TARGET pixelboost_shared)

add_subdirectory (engine)
pixelboost_setup_library(pixelboost_engine)
