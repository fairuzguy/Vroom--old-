cmake_minimum_required(VERSION 3.4.1)
project(Triangle)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../third-party")
set(MAIN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../main/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

set(LIB_SDL2 ${THIRD_PARTY_DIR}/sdl-windows/lib/x64/SDL2.lib)
set(LIB_SDL2_MAIN ${THIRD_PARTY_DIR}/sdl-windows/lib/x64/SDL2main.lib)
set(LIB_GLEW ${THIRD_PARTY_DIR}/glew/lib/Release/x64/glew32s.lib)
set(LIB_SDL2_IMAGE ${THIRD_PARTY_DIR}/sdl-image-windows/lib/x64/SDL2_image.lib)

include_directories(${THIRD_PARTY_DIR}/sdl-windows/include)
include_directories(${THIRD_PARTY_DIR}/glew/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/sdl-image-windows/include)
file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

if(CMAKE_BUILD_TYPE MATCHES "Release")
	set(EXECUTABLE_TYPE WIN32)
endif()

add_executable(
	Triangle ${EXECUTABLE_TYPE}
	${CPP_HEADERS}
	${CPP_SOURCES}
)

set_property(TARGET Triangle PROPERTY CXX_STANDARD 17)
set_property(TARGET Triangle PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(
	Triangle
	opengl32
	${LIB_GLEW}
	${LIB_SDL2}
	${LIB_SDL2_MAIN}
	${LIB_SDL2_IMAGE}
)

add_custom_command(
	TARGET Triangle
	POST_BUILD
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND PowerShell (Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted) $<SEMICOLON> (\.\\cmake-post-build.ps1)
)
