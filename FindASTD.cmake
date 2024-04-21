#  ASTD_FOUND - system has ASTD
#  ASTD_INCLUDE_DIRS - the ASTD include directory
#  ASTD_LIBRARIES - The libraries needed to use ASTD

find_path(ASTD_INCLUDE_DIRS
		NAMES ASTD.h
		PATH_SUFFIXES include
)

if (ASTD_INCLUDE_DIRS)
	set(HAVE_ASTD_H 1)
endif ()

# No libraries required, is header-only. No dependency
# Keeping this here for consistency
set(ASTD_LIBRARIES "")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASTD
		DEFAULT_MSG
		ASTD_LIBRARIES ASTD_INCLUDE_DIRS
		HAVE_ASTD_H
)
