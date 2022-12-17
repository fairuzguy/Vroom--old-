#!/bin/bash

source ../shared-scripts.sh

ANDROID_SDK="/Users/$USER/Library/Android/sdk"
echo "Using Android SDK at: $ANDROID_SDK"

export JAVA_HOME="/Library/Java/JavaVirtualMachines/openjdk-8.jdk/Contents/Home" #"/Applications/Android Studio.app/Contents/jre/jdk/Contents/Home"

NDK_VERSION="20.0.5594570"
ANDROID_NDK="$ANDROID_SDK/ndk/$NDK_VERSION"
echo "Using Android NDK at: $ANDROID_NDK"

echo "Auto accepting Android SDK licenses..."
yes | $ANDROID_SDK/tools/bin/sdkmanager --licenses

echo "Installing required Android SDK components..."
$ANDROID_SDK/tools/bin/sdkmanager \
	"platform-tools" \
	"build-tools;28.0.3" \
	"tools" \
	"platforms;android-28" \
	"cmake;3.10.2.4988404" \
	"ndk;$NDK_VERSION"

fetch_brew_dependency "wget"
fetch_third_party_lib_sdl "2.0.9"
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_sdl_image

pushd sdl/jni
	if [ ! -d "SDL" ]; then
		echo "Linkding SDL path to sdl/jni/SDL..."
		ln -s ../../../../third-party/SDL SDL
	fi
popd

pushd sdl/src/main
	echo "Copying base SDL Java source to Android library project..."
	cp -r ../../../../../third-party/SDL/android-project/app/src/main/java .
popd

pushd app/src/main
	if [ ! -d "assets" ]; then
		echo "Creating root 'assets' folder in app/src/main..."
		mkdir assets
	fi
popd

pushd app/src/main/assets
	if [ ! -d "assets" ]; then
		echo "Linking assets to ../../../../../main/assets"
		ln -s ../../../../../main/assets assets
	fi
popd

# Fixing a bug in the SDL2_image library
pushd ../../third-party/SDL2_image
	echo "Disabling SDL2_image webp integration..."
	sed -i '' -e 's/SUPPORT_WEBP ?= true/SUPPORT_WEBP ?= false/g' Android.mk
popd

pushd sdl/jni
	if [ ! -d "SDL2_image" ]; then
		echo "Linking SDL2_image path to sdl/jni/SDL2_image..."
		ln -s ../../../../third-party/SDL2_image SDL2_image
	fi
popd
