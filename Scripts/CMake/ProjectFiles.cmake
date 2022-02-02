
function(ASTD_file_public_directories CURRENT_DIR DIRECTORIES)

set(${DIRECTORIES}
    "${CURRENT_DIR}/../../ASTD/Source/Public"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Macros"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Macros/Core"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Macros/Ensure"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Platform"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Platform/Linux"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Platform/Win32"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Shared"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Shared/Misc"
    "${CURRENT_DIR}/../../ASTD/Source/Public/Shared/Types"
    "${CURRENT_DIR}/../../ASTD/Source/Public/TypeTraits"
    "${CURRENT_DIR}/../../ASTD/Source/Public/TypeTraits/Core"
    "${CURRENT_DIR}/../../ASTD/Source/Public/TypeTraits/Misc"
    PARENT_SCOPE)

endfunction()


function(ASTD_file_private_files CURRENT_DIR FILES)

file(GLOB_RECURSE "PRIVATE_FILES"
    "${CURRENT_DIR}/../../ASTD/Source/Private/*.cpp" 
)

set(${FILES} ${PRIVATE_FILES} PARENT_SCOPE)

endfunction()
