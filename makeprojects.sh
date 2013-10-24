#!/bin/bash

rm -r build

mkdir build
cd build

mkdir ios
cd ios
cp -r ../../pixelboost/engine/resources/platform/ios/*.png .
if [ -d "../../platform/ios" ]; then
	cp -r ../../platform/ios/*.png .
fi
cmake -G Xcode ../../ -DPIXELBOOST_BUILD_PLATFORM_IOS=TRUE
cd ..

mkdir osx
cd osx
cmake -G Xcode ../../ -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..

declare -a androidplatforms=(ajagplay rowgplay ajakindle rowkindle)

for platform in ${androidplatforms[@]}
do
	mkdir android_release_$platform
	cd android_release_$platform
	android create project --name pixelboost --target android-16 \
	   --package com.pixelballoon.pixelboost --activity PixelboostActivity \
	   --path .
	mkdir assets
	cd assets
	mkdir res
	mkdir values
	cd ..
	cp -r ../../pixelboost/engine/resources/platform/android/res/drawable* res/
	if [ -d "../../platform/android/res/drawable-hdpi" ]; then
	   cp -r ../../platform/android/res/drawable* res/
	fi
	cp -r ../../pixelboost/engine/src/platform/android/src .
	cp -r ../../pixelboost/libs/ouya/* libs

	if [[ $platform == *kindle ]]
	then
		ANDROID_STORE_TYPE=AMAZON
	else
		ANDROID_STORE_TYPE=GOOGLEPLAY
	fi

	cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_ANDROID=TRUE -DANDROID_STL=gnustl_static -DLIBRARY_OUTPUT_PATH_ROOT="`pwd`" -DCMAKE_TOOLCHAIN_FILE=pixelboost/toolchain/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DPIXELBOOST_ANDROID_TARGET="$platform" -DPIXELBOOST_ANDROID_STORE=$ANDROID_STORE_TYPE
	cd ..
done

mkdir android_debug
cd android_debug
android create project --name pixelboost --target android-16 \
	--package com.pixelballoon.pixelboost --activity PixelboostActivity \
	--path .
mkdir assets
cd assets
mkdir res
mkdir values
cd ..
cp -r ../../pixelboost/engine/resources/platform/android/res/drawable* res/
if [ -d "../../platform/android/res/drawable-hdpi" ]; then
	cp -r ../../platform/android/res/drawable* res/
fi
cp -r ../../pixelboost/engine/src/platform/android/src .
cp -r ../../pixelboost/libs/ouya/* libs
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_ANDROID=TRUE -DANDROID_STL=gnustl_static -DLIBRARY_OUTPUT_PATH_ROOT="`pwd`" -DCMAKE_TOOLCHAIN_FILE=pixelboost/toolchain/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DPIXELBOOST_ANDROID_TARGET="debug"
cd ..

mkdir emscripten_release
cd emscripten_release
cp -r ../../data ./
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN=TRUE -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Platform/Emscripten_unix.cmake -DCMAKE_MODULE_PATH=$EMSCRIPTEN/cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cd ..

mkdir emscripten_debug
cd emscripten_debug
cp -r ../../data ./
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN=TRUE -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Platform/Emscripten_unix.cmake -DCMAKE_MODULE_PATH=$EMSCRIPTEN/cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
cd ..

cd ..
cd pixelboost
cd tools
cd editor
rm -r build
mkdir build
cd build
cmake -G Xcode .. -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..
cd ..
cd gametool
rm -r build
mkdir build
cd build
cmake -G Xcode .. -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..
cd ..
cd modeltool
rm -r build
mkdir build
cd build
cmake -G Xcode .. -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..
cd ..
cd shadertool
rm -r build
mkdir build
cd build
cmake -G Xcode .. -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..
cd ..
cd texturetool
rm -r build
mkdir build
cd build
cmake -G Xcode .. -DPIXELBOOST_BUILD_PLATFORM_OSX=TRUE
cd ..
cd ..
