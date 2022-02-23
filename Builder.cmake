
# setups build for cmake
function(builder_setup_build)

	# Configuration
	# -----------------------------------------------------------

	set(CMAKE_CXX_STANDARD 17)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -fpermissive")
	set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE)

	# Build type
	# -----------------------------------------------------------

	set(DEBUG_POSTFIX "-Debug")
	set(RELEASE_POSTFIX "-Release")

	# Platform
	# -----------------------------------------------------------
	
	if(MSVC)
		add_link_options("/NODEFAULTLIB:MSVCRT")
	endif(MSVC)

	# Definitions
	# -----------------------------------------------------------

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_compile_definitions("_DEBUG=1")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_compile_definitions("_RELEASE=1")
	else()
		# call ERROR!
	endif()

endfunction()

# add_build_target
# @param - build option (EXE/EXECUTABLE, LIB/LIBRARY)
# @param - name of the project
# @param - name of module
# @param - array of definitions
# @param - array of include directories
# @param - array of source files
function(builder_add_build_module BUILD_OPTION MODULE_NAME MODULE_PATH DEFINITIONS INCLUDE_DIRS SOURCES LINK_LIBRARIES)

	# Output dirs
	# -----------------------------------------------------------

	if(WIN32)
		set(BUILD_PLATFORM "Win32")
	elseif(UNIX)
		set(BUILD_PLATFORM "Linux")
	else()
		message(FATAL_ERROR "Unsupported platform [${BUILD_PLATFORM}]")
	endif()
		
	# example: C:/MyProject/MyModule/Binaries/Win32/Debug
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MODULE_PATH}/Binaries/${BUILD_PLATFORM}/${CMAKE_BUILD_TYPE}")
	
	# example: C:/MyProject/MyModule/Intermediate/Win32/Debug
	set(CMAKE_CURRENT_BINARY_DIR "${MODULE_PATH}/Intermediate/${BUILD_PLATFORM}/${CMAKE_BUILD_TYPE}")
	
	# Target
	# -----------------------------------------------------------
	
	if(BUILD_OPTION STREQUAL "EXECUTABLE" OR BUILD_OPTION STREQUAL "EXE")
		add_executable(${MODULE_NAME} "${SOURCES}")
	elseif(BUILD_OPTION STREQUAL "LIBRARY" OR BUILD_OPTION STREQUAL "LIB")
		add_library(${MODULE_NAME} "${SOURCES}")
	else()
		message(FATAL_ERROR "Unsupported build option [${BUILD_OPTION}]")
	endif()

	target_compile_definitions(${MODULE_NAME} PRIVATE "${DEFINITIONS}")
	target_include_directories(${MODULE_NAME} PRIVATE "${INCLUDE_DIRS}")
	target_link_libraries(${MODULE_NAME} PRIVATE "${LINK_LIBRARIES}")

endfunction()