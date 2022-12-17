#!/bin/bash

source ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

fetch_third_party_lib_sdl "2.0.14"
fetch_framework_sdl2 "2.0.14"
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_sdl_image
fetch_framework_sdl2_image

if [ ! -d "Source" ]; then
	echo "Linking 'Source' path to '../main/src"
	ln -s ../main/src Source
fi

if [ ! -d "assets" ]; then
	echo "Linking 'assets' path to '../main/assets'"
	ln -s ../main/assets assets
fi

echo "Generating Xcode project"
xcodegen generate
