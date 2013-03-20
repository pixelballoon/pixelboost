cmake_minimum_required (VERSION 2.8.10)

set (PHYSFS_ARCHIVE_7Z FALSE CACHE BOOL "Enable 7zip support")
set (PHYSFS_BUILD_SHARED FALSE CACHE BOOL "Build physfs shared library")
set (PHYSFS_BUILD_TEST FALSE CACHE BOOL "Build stdio test program.")

add_subdirectory (libs/box2d)
add_subdirectory (libs/enet)
add_subdirectory (libs/freetype2)
add_subdirectory (libs/lua)
add_subdirectory (libs/physfs)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	add_subdirectory (libs/ddhidlib)
endif (PIXELBOOST_BUILD_PLATFORM_OSX)

if (NOT TARGET pixelboost_shared)
	add_subdirectory (shared)
endif (NOT TARGET pixelboost_shared)
add_subdirectory (engine)
