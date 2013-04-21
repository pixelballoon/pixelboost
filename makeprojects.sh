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

mkdir android
cd android
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
cp -r ../../data assets
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_ANDROID=TRUE -DANDROID_STL=gnustl_static -DLIBRARY_OUTPUT_PATH_ROOT="`pwd`" -DCMAKE_TOOLCHAIN_FILE=pixelboost/toolchain/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cd ..

mkdir emscripten
cd emscripten
cp -r ../../data ./
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN=TRUE -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Platform/Emscripten_unix.cmake -DCMAKE_MODULE_PATH=$EMSCRIPTEN/cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cd ..

mkdir emscripten_dbg
cd emscripten_dbg
cp -r ../../data ./
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN=TRUE -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Platform/Emscripten_unix.cmake -DCMAKE_MODULE_PATH=$EMSCRIPTEN/cmake -G "Unix Makefiles"
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
