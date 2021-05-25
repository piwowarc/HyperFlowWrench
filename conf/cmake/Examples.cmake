# list of examples
set(EXAMPLES_CMAKEFILES_TXT
        examples/lambda-bag-of-tasks/CMakeLists.txt
        )

foreach (cmakefile ${EXAMPLES_CMAKEFILES_TXT})
    string(REPLACE "/CMakeLists.txt" "" repository ${cmakefile})
    add_subdirectory("${CMAKE_HOME_DIRECTORY}/${repository}")
endforeach ()

# install example files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples
        DESTINATION wrench
        FILES_MATCHING PATTERN "*.json" PATTERN "*.xml" PATTERN "*.dax"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )
