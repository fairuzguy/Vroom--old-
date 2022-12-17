#!/bin/bash

echo "Adding Frameworks @rpath to executable"
install_name_tool -add_rpath @loader_path/../Frameworks out/Triangle
t/Triangle-console

pushd out
	if [ ! -d "assets" ]; then
		echo "Linking 'assets' path to '../../main/assets'"
		ln -s ../../main/assets assets
	fi
popd
