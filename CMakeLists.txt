cmake_minimum_required(VERSION 3.22)

project(ASTD VERSION 1.0 DESCRIPTION "Alternity STD")

add_library(ASTD INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} INTERFACE
		$<$<CONFIG:Debug>:_DEBUG=1>
)
