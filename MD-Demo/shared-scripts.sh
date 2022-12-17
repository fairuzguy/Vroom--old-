#!/bin/bash

function fetch_brew_dependency() {
	FORMULA_NAME=$1
	echo "Grabbing brew dependency '$FORMULA_NAME'."
	if brew ls --versions $FORMULA_NAME > /dev/null; then
		echo "Dependency '$FORMULA_NAME' is already installed."
	else
		echo "Installing dependency '$FORMULA_NAME' via Homebrew..."
		brew install $FORUMA_NAME
	fi
}

function verify_third_party_folder_exists() {
	pushd ../../
		if [ ! -d "third-party" ]; then
			mkdir third-party
		fi
	popd
}

function fetch_third_party_lib_sdl() {
	VERSION_NUMBER=$1

	verify_third_party_folder_exists
	pushd ../../third-party
		if [ ! -d "SDL" ]; then
			echo "Fetching SDL2-$VERSION_NUMBER"
			wget https://www.libsdl.org/release/SDL2-$VERSION_NUMBER.zip
			unzip -q "SDL2-$VERSION_NUMBER.zip"
			mv "SDL2-$VERSION_NUMBER" SDL
			rm "SDL2-$VERSION_NUMBER.zip"
		else
			echo "SDL library already exists. If you want to update to a desired version, remove the current SDL library and update setup.sh in the project directory for each target to request the current version."
		fi
	popd
}

function fetch_third_party_lib_sdl_image() {
	verify_third_party_folder_exists
	pushd ../../third-party
		if [ ! -d "SDL2_image" ]; then
			echo "Fetching SDL2_image source library (2.0.4)..."
			wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.zip
			unzip -q SDL2_image-2.0.4.zip
			mv SDL2_image-2.0.4 SDL2_image
			rm SDL2_image-2.0.4.zip
		fi
	popd
}

function verify_frameworks_folder_exists() {
	if [ ! -d "Frameworks" ]; then
		mkdir Frameworks
	fi
}

function fetch_framework_sdl2() {
	FRAMEWORK_VERSION=$1
	verify_frameworks_folder_exists
	pushd Frameworks
		if [ ! -d "SDL2.framework" ]; then
			wget https://www.libsdl.org/release/SDL2-$FRAMEWORK_VERSION.dmg
			echo "Mounting SDL2-$FRAMEWORK_VERSION.dmg"
			hdiutil attach SDL2-$FRAMEWORK_VERSION.dmg
			echo "Copying SDL2.framework into current folder"
			cp -R /Volumes/SDL2/SDL2.framework .
			echo "Unmounting DMG"
			hdiutil detach /Volumes/SDL2
			echo "Deleting DMG"
			rm SDL2-$FRAMEWORK_VERSION.dmg

			pushd SDL2.framework
				echo "Code signing SDL2.framework"
				codesign -f -s - SDL2
			popd
		else
			echo "SDL2.framework already exists!"
		fi
	popd
}

function fetch_framework_sdl2_image() {
	verify_frameworks_folder_exists
	pushd Frameworks
		if [ ! -d "SDL2_image.framework" ]; then
			echo "Fetching SDL2_image framework from: https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg"
			wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg
			echo "Attaching DMG file..."
			hdiutil attach SDL2_image-2.0.4.dmg
			echo "Copying SDL2_image.framework from DMG file..."
			cp -R /Volumes/SDL2_image/SDL2_image.framework .
			echo "Detaching DMG file..."
			hdiutil detach /Volumes/SDL2_image
			rm SDL2_image-2.0.4.dmg

			pushd SDL2_image.framework/Versions/A/Frameworks/webp.framework
				echo "Code signing SDL2_image.framework/Frameworks/webp.framework..."
				codesign -f -s - webp
			popd

			pushd SDL2_image.framework
				echo "Code signing SDL2_image.framework..."
				codesign -f -s - SDL2_image
			popd
		fi
	popd
}

function verify_build_folder_exists() {
	if [ ! -d "build" ]; then
		mkdir build
	fi
}

function fetch_third_party_lib_glm() {
	verify_third_party_folder_exists
	pushd ../../third-party
		if [ ! -d "glm" ]; then
			echo "Fetching GLM from: https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip"
			wget https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip
			unzip -q glm-0.9.9.3.zip
			rm glm-0.9.9.3.zip
		fi
	popd
}

function fetch_third_party_lib_tiny_obj_loader() {
	verify_third_party_folder_exists
	pushd ../../third-party
		if [ ! -d "tiny-obj-loader" ]; then
			echo "Fetching Tiny OBJ Loader from: https://github.com/syoyo/tinyobjloader/archive/v1.4.1.zip"
			echo "Tiny OBJ Loader header file needs to be downloaded manually from master branch."
		fi
	popd
}
