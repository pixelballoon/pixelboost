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
cp -r ../../data assets
cmake ../../ -DPIXELBOOST_BUILD_PLATFORM_ANDROID=TRUE -DANDROID_STL=gnustl_static -DCMAKE_TOOLCHAIN_FILE=pixelboost/toolchain/android.toolchain.cmake
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
