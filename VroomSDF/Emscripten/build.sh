#!/bin/bash

source ../shared-scripts.sh

if [ ! -d "build" ]; then
	mkdir build
fi

if [ -d "out" ]; then
	rm -rf out
fi

pushd ../../third-party/emscripten
	echo "Configuring Emscripten environment variables"
	. ./emsdk_env.sh
popd

pushd build
	echo "The EMSCRIPTEN environment variable (if it's blank it's weird...): ${EMSCRIPTEN}"
	EMSCRIPTEN_CMAKE_PATH=../../../third-party/emscripten/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
	echo "Emscripten CMake path: ${EMSCRIPTEN_CMAKE_PATH}"
	cmake -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN_CMAKE_PATH} ..

	echo "Building project..."
	make
popd
