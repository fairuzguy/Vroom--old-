#!/bin/bash

source ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"
fetch_brew_dependency "ninja"

fetch_third_party_lib_sdl "2.0.9"
fetch_framework_sdl2 "2.0.9"
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_sdl_image
fetch_framework_sdl2_image
