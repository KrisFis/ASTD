
function(ASTD_file_public_directories CURRENT_DIR DIRECTORIES)

set(${DIRECTORIES}
    "${CURRENT_DIR}/../../ASTD/Source/Public"
    PARENT_SCOPE)

endfunction()

function(ASTD_file_private_directories CURRENT_DIR DIRECTORIES)

set(${DIRECTORIES}
    "${CURRENT_DIR}/../../ASTD/Source/Private"
    PARENT_SCOPE)

endfunction()

function(ASTD_file_private_files CURRENT_DIR FILES)

file(GLOB_RECURSE "PRIVATE_FILES"
    "${CURRENT_DIR}/../../ASTD/Source/Private/*.cpp" 
)

set(${FILES} ${PRIVATE_FILES} PARENT_SCOPE)

endfunction()
