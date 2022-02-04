# Setup
# -----------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

project(ASTD)

# Compiler
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -fpermissive")

# Paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Binaries")
set(CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}/Intermediate")

# Configuration types
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE)

# Post fix
set(DEBUG_POSTFIX "-Debug")
set(RELEASE_POSTFIX "-Release")

# fix for MSVC lib conflict
if(MSVC)
    add_link_options("/NODEFAULTLIB:MSVCRT")
endif(MSVC)

# Outputs
# -----------------------------------------------------------

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Win32)
    set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Win32)
elseif(UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Unix)
    set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Unix)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Release)
endif()

# Includes
# -----------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/Scripts/CMake/Configuration.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Scripts/CMake/ProjectFiles.cmake)

# Definitions
# -----------------------------------------------------------

ASTD_definitions(${CMAKE_BUILD_TYPE} ALL_DEFINITIONS)

add_definitions(${ALL_DEFINITIONS})

# Sources
# -----------------------------------------------------------

ASTD_file_public_directories(${CMAKE_CURRENT_LIST_DIR}/Scripts/CMake ALL_PUBLIC_INCLUDE_DIRS)
ASTD_file_private_directories(${CMAKE_CURRENT_LIST_DIR}/Scripts/CMake ALL_PRIVATE_INCLUDE_DIRS)

ASTD_file_private_files(${CMAKE_CURRENT_LIST_DIR}/Scripts/CMake ALL_SOURCE_FILES)

include_directories("${ALL_PUBLIC_INCLUDE_DIRS}" "${ALL_PRIVATE_INCLUDE_DIRS}")

# Executable and linking
# -----------------------------------------------------------

add_library(${PROJECT_NAME} "${ALL_SOURCE_FILES}")
#add_executable(${PROJECT_NAME} "${ALL_SOURCE_FILES}")