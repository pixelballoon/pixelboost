include (config.cmake)

macro(pixelboost_library_cpp11 library)
	if (PIXELBOOST_BUILD_PLATFORM_OSX OR PIXELBOOST_BUILD_PLATFORM_IOS)
		set_target_properties(${library} PROPERTIES
			XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++0x"
			XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
			)

		if (PIXELBOOST_BUILD_PLATFORM_OSX)
			set_target_properties(${library} PROPERTIES
				XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET 10.7
				)
		endif (PIXELBOOST_BUILD_PLATFORM_OSX)

		if (PIXELBOOST_BUILD_PLATFORM_IOS)
			set_target_properties(${library} PROPERTIES
				XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 5.0
				)
		endif (PIXELBOOST_BUILD_PLATFORM_IOS)
	endif (PIXELBOOST_BUILD_PLATFORM_OSX OR PIXELBOOST_BUILD_PLATFORM_IOS)
endmacro()

macro(pixelboost_setup_pre pixelboost_dir projectname identifier title)
	set (PIXELBOOST_ROOT_DIR "${CMAKE_SOURCE_DIR}/${pixelboost_dir}")
	set (PIXELBOOST_BUILD_NAME ${projectname})
	set (PIXELBOOST_BUILD_IDENTIFIER ${identifier})
	set (PIXELBOOST_BUILD_TITLE ${title})

	add_definitions(-DPIXELBOOST_BUILD_NAME="${projectname}")
	add_definitions(-DPIXELBOOST_BUILD_IDENTIFIER="${identifier}")
	add_definitions(-DPIXELBOOST_BUILD_TITLE="${title}")

	option (PIXELBOOST_BUILD_PLATFORM_ANDROID "Build for Android Platform" FALSE)
	option (PIXELBOOST_BUILD_PLATFORM_IOS "Build for iOS Platform" FALSE)
	option (PIXELBOOST_BUILD_PLATFORM_OSX "Build for OSX Platform" FALSE)

	if (PIXELBOOST_LIBRARY_USE_GWEN)
		set (PIXELBOOST_LIBRARY_HEADERS
			"${PIXELBOOST_LIBRARY_HEADERS}"
			"${PIXELBOOST_ROOT_DIR}/libs/gwen/gwen/include"
			)
		add_definitions(-DPIXELBOOST_LIBRARY_USE_GWEN)
		add_subdirectory(${PIXELBOOST_ROOT_DIR}/libs/gwen ${CMAKE_BINARY_DIR}/gwen)
	endif (PIXELBOOST_LIBRARY_USE_GWEN)

	if (PIXELBOOST_BUILD_PLATFORM_OSX)
		add_definitions(-DPIXELBOOST_DISABLE_GAMECENTER
						-DPIXELBOOST_DISABLE_SOUND
						-DPIXELBOOST_PLATFORM_OSX)

		set(RESOURCES ${RESOURCES}
			${PIXELBOOST_DATA_DIRS})

		set(RESOURCES ${RESOURCES}
			${PIXELBOOST_ROOT_DIR}/engine/resources/platform/osx/MainView.xib
			${PIXELBOOST_ROOT_DIR}/engine/resources/platform/osx/MainMenu.xib
			)
	
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ObjC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
	endif (PIXELBOOST_BUILD_PLATFORM_OSX)

	if (PIXELBOOST_BUILD_PLATFORM_IOS)
		add_definitions(-DPIXELBOOST_DISABLE_TESTFLIGHT
						-DPIXELBOOST_PLATFORM_IOS)

		set(RESOURCES ${RESOURCES}
			${PIXELBOOST_DATA_DIRS})

		set(RESOURCES ${RESOURCES}
			${PIXELBOOST_ROOT_DIR}/engine/resources/platform/ios/MainWindow.xib 
			${PIXELBOOST_ROOT_DIR}/engine/resources/platform/ios/MainWindow-iPad.xib 
			${PIXELBOOST_ROOT_DIR}/engine/resources/platform/ios/pixelboostViewController.xib
			${CMAKE_BINARY_DIR}/Default-568h@2x.png
			${CMAKE_BINARY_DIR}/Default-Landscape@2x~ipad.png
			${CMAKE_BINARY_DIR}/Default-Landscape~ipad.png
			${CMAKE_BINARY_DIR}/Default-Portrait@2x~ipad.png
			${CMAKE_BINARY_DIR}/Default-Portrait~ipad.png
			${CMAKE_BINARY_DIR}/Default.png
			${CMAKE_BINARY_DIR}/Default@2x.png
			${CMAKE_BINARY_DIR}/Icon-72.png
			${CMAKE_BINARY_DIR}/Icon-72@2x.png
			${CMAKE_BINARY_DIR}/Icon.png
			${CMAKE_BINARY_DIR}/Icon@2x.png
			)

		set(CMAKE_OSX_SYSROOT iphoneos)
		set(CMAKE_OSX_ARCHITECTURES "armv7;armv7s;i386")
		set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ObjC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
	endif (PIXELBOOST_BUILD_PLATFORM_IOS)

	if (PIXELBOOST_BUILD_PLATFORM_ANDROID)
		add_definitions(-DPIXELBOOST_DISABLE_GAMECENTER
						-DPIXELBOOST_PLATFORM_ANDROID
						-D_GLIBCXX_HAS_GTHREADS)

		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

		configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pixelboost/engine/resources/platform/android/custom_rules.xml" "${CMAKE_CURRENT_SOURCE_DIR}/build/android/custom_rules.xml" @ONLY)
		configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pixelboost/engine/resources/platform/android/AndroidManifest.xml" "${CMAKE_CURRENT_SOURCE_DIR}/build/android/AndroidManifest.xml")
		configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pixelboost/engine/resources/platform/android/res/values/strings.xml" "${CMAKE_CURRENT_SOURCE_DIR}/build/android/res/values/strings.xml")
	endif (PIXELBOOST_BUILD_PLATFORM_ANDROID)

	add_subdirectory (${PIXELBOOST_ROOT_DIR} ${CMAKE_BINARY_DIR}/pixelboost)

	include_directories("${PIXELBOOST_HEADERS}")

	file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
endmacro()

macro(pixelboost_setup_post sources)
	if (NOT PIXELBOOST_BUILD_PLATFORM_ANDROID)
		add_executable(${PIXELBOOST_BUILD_NAME} ${sources} ${RESOURCES})
		target_link_libraries (${PIXELBOOST_BUILD_NAME} pixelboost_engine)
	endif (NOT PIXELBOOST_BUILD_PLATFORM_ANDROID)

	if (PIXELBOOST_BUILD_PLATFORM_ANDROID)
		add_library(pixelboost SHARED ${sources}
			"${PIXELBOOST_ROOT_DIR}/engine/src/platform/android/pixelboost/framework/main.cpp")
		target_link_libraries (pixelboost pixelboost_engine)
	endif (PIXELBOOST_BUILD_PLATFORM_ANDROID)

	if (PIXELBOOST_BUILD_PLATFORM_OSX)
		set_target_properties(${PIXELBOOST_BUILD_NAME} PROPERTIES
			MACOSX_BUNDLE_GUI_IDENTIFIER ${PIXELBOOST_BUILD_IDENTIFIER}
			MACOSX_BUNDLE_INFO_PLIST "${PIXELBOOST_ROOT_DIR}/engine/resources/platform/osx/Info.plist.in"
			MACOSX_BUNDLE TRUE
			RESOURCE "${RESOURCES}"
			XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES YES
			XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
			XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
			XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
			XCODE_ATTRIBUTE_SKIP_INSTALL "No"
		)
	endif (PIXELBOOST_BUILD_PLATFORM_OSX)

	if (PIXELBOOST_BUILD_PLATFORM_IOS)
		set_target_properties(${PIXELBOOST_BUILD_NAME} PROPERTIES
			MACOSX_BUNDLE_GUI_IDENTIFIER ${PIXELBOOST_BUILD_IDENTIFIER}
			MACOSX_BUNDLE_INFO_PLIST "${PIXELBOOST_ROOT_DIR}/engine/resources/platform/ios/Info.plist.in"
			MACOSX_BUNDLE TRUE
			RESOURCE "${RESOURCES}"
			XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
			XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
			XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES YES
			XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
			XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 5.0
			XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # Universal app
			XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
			XCODE_ATTRIBUTE_SKIP_INSTALL "No"
			XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++0x"
			XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
		)
	endif (PIXELBOOST_BUILD_PLATFORM_IOS)

	pixelboost_library_cpp11(${PIXELBOOST_BUILD_NAME})

	if (PIXELBOOST_LIBRARY_USE_GWEN)
		target_link_libraries (${PIXELBOOST_BUILD_NAME} gwen)
		pixelboost_library_cpp11(gwen)
	endif (PIXELBOOST_LIBRARY_USE_GWEN)
endmacro()