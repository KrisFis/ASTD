cmake_minimum_required(VERSION 3.10)

project(ASTD)

# Init
# -----------------------------------------------------------

set(ASTD_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/ASTD")
set(TESTPROJ_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/TestProj")

# Includes
# -----------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/Builder.cmake")

include("${ASTD_MODULE_PATH}/ASTDBuild.cmake")
include("${TESTPROJ_MODULE_PATH}/TestProjBuild.cmake")

# Setup
# -----------------------------------------------------------

builder_setup_build()

# Includes && Sources
# -----------------------------------------------------------

ASTD_public_include_directories(${ASTD_MODULE_PATH} ASTD_INCLUDE_DIRS)
TestProj_public_include_directories(${TESTPROJ_MODULE_PATH} TESTPROJ_INCLUDE_DIRS)

ASTD_public_sources(${ASTD_MODULE_PATH} ASTD_SOURCES)
TestProj_public_sources(${TESTPROJ_MODULE_PATH} TESTPROJ_SOURCES)

set(ALL_INCLUDE_DIRS "${ASTD_INCLUDE_DIRS}" "${TESTPROJ_INCLUDE_DIRS}")
set(ALL_SOURCES "${ASTD_SOURCES}" "${TESTPROJ_SOURCES}")

# Module build
# -----------------------------------------------------------

builder_add_build_module(EXECUTABLE "TestProj" "${TESTPROJ_MODULE_PATH}" "" "${ALL_INCLUDE_DIRS}" "${ALL_SOURCES}" "")