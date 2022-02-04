
function(ASTD_definitions BUILD_TYPE DEFINITIONS)

if(BUILD_TYPE STREQUAL "Debug")
	set(${DEFINITIONS}
		"-D_DEBUG=1"
		"-DDO_ENSURES=1"
	PARENT_SCOPE)
elseif(BUILD_TYPE STREQUAL "Release")
	set(${DEFINITIONS}
		"-D_RELEASE=1"
		"-DDO_ENSURES=0"
	PARENT_SCOPE)
endif()

endfunction()