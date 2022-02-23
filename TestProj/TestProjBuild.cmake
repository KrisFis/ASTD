
function(TestProj_public_include_directories CURRENT_DIR RETURN_VAL)

	set(${RETURN_VAL} "${CURRENT_DIR}/Source/Public" PARENT_SCOPE)

endfunction()

function(TestProj_public_sources CURRENT_DIR RETURN_VALS)

	file(GLOB_RECURSE "SOURCES" "${CURRENT_DIR}/Source/Private/*.cpp")

	set(${RETURN_VALS} ${SOURCES} PARENT_SCOPE)

endfunction()