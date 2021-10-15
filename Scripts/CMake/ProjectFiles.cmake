
function(ASTD_include_directories CURRENT_DIR)

include_directories(
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Macros"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Macros/Core"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Macros/Ensure"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Primitives"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Primitives/Misc"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Shared"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Shared/Misc"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/Shared/Types"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/TypeTraits"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/TypeTraits/Core"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Public/TypeTraits/Misc")

endfunction()


function(ASTD_file_sources CURRENT_DIR SOURCES)

file(GLOB_RECURSE "ASTD_SOURCES"
    "${CURRENT_DIR}/../../${PROJECT_NAME}/Source/Private/*.cpp" 
)

set(${SOURCES} ${ASTD_SOURCES} PARENT_SCOPE)

endfunction()
