#!/bin/bash

source ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

fetch_third_party_lib_sdl "2.0.9"
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader

function create_static_library() {
	STATIC_LIBRARY=$1
	PROJECT_PATH=$2
	XCODE_PROJECT=$3
	XCODE_TARGET=$4
	BUILD_FOLDER=$5

	if [ ! -d "Libs" ]; then
		mkdir Libs
	fi

	pushd Libs
		if [ ! -e libSDL2.a ]; then
			pushd $PROJECT_PATH
				echo "Building iOS iPhone static library"
				xcrun xcodebuild -configuration "Release" \
					-project $XCODE_PROJECT \
					-target $XCODE_TARGET \
					-sdk "iphoneos" \
					build \
					ONLY_ACTIVE_ARCH=NO \
					RUN_CLANG_STATIC_ANALYZER=NO \
					BUILD_DIR="build/$BUILD_FOLDER" \
					SYMROOT="build/$BUILD_FOLDER" \
					OBJROOT="build/$BUILD_FOLDER/obj" \
					BUILD_ROOT="build/$BUILD_FOLDER" \
					TARGET_BUILD_DIR="build/$BUILD_FOLDER/iphoneos"

				echo "Building iOS Simulator static library"
				xcrun xcodebuild -configuration "Release" \
					-project $XCODE_PROJECT \
					-target $XCODE_TARGET \
					-sdk "iphonesimulator" \
					-arch x86_64 \
					build \
					ONLY_ACTIVE_ARCH=NO \
					RUN_CLANG_STATIC_ANALYZER=NO \
					BUILD_DIR="build/$BUILD_FOLDER" \
					SYMROOT="build/$BUILD_FOLDER" \
					OBJROOT="build/$BUILD_FOLDER/obj" \
					BUILD_ROOT="build/$BUILD_FOLDER" \
					TARGET_BUILD_DIR="build/$BUILD_FOLDER/iphonesimulator"

				echo "Creating fat library"
				xcrun -sdk iphoneos lipo -create \
					-output "build/$BUILD_FOLDER/$STATIC_LIBRARY" \
					"build/$BUILD_FOLDER/iphoneos/$STATIC_LIBRARY" \
					"build/$BUILD_FOLDER/iphonesimulator/$STATIC_LIBRARY"

				echo "The fat static library '$STATIC_LIBRARY' is ready."
			popd

			echo "Copying '$STATIC_LIBRARY' into Libs"
			cp "$PROJECT_PATH/build/$BUILD_FOLDER/$STATIC_LIBRARY" .
		fi
	popd

}

create_static_library libSDL2.a ../../../third-party/SDL/Xcode-iOS/SDL \
	SDL.xcodeproj libSDL-iOS SDL

create_static_library libSDL2_image.a ../../../third-party/SDL2_image/Xcode-iOS \
	SDL_image.xcodeproj libSDL_image-iOS SDL_image

if [ ! -d "Source" ]; then
	echo "Linking 'Source' path to '../main/src'"
	ln -s ../main/src Source
fi

if [ ! -d "assets" ]; then
	echo "Linking 'assets' path to '../main/assets'"
	ln -s ../main/assets assets
fi

echo "Generating Xcode project"
xcodegen generate
