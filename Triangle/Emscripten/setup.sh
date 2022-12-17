#!/bin/bash

source ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"
fetch_third_party_lib_sdl "2.0.9"
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_sdl_image

pushd ../../third-party
	if [ ! -d "emscripten" ]; then
		echo "Fetching Emscripten SDK"
		wget https://github.com/emscripten-core/emsdk/archive/master.zip
		unzip -q master.zip

		mv emsdk-master emscripten
		rm master.zip

		pushd emscripten
			echo "Updating Emscripten SDK"
			./emsdk update
			echo "Installing lastest Emscripten SDK"
			./emsdk install latest
			echo "Activating latest Emscripten SDK"
			./emsdk activate latest
		popd
	fi
popd
