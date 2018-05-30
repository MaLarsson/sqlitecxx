# Distributed under the OSI-approved MIT License.  See accompanying
# file LICENSE or https://github.com/Crascit/DownloadProject for details.

set(_DownloadProjectDir "${CMAKE_CURRENT_LIST_DIR}")

include(CMakeParseArguments)

function(download_project)

    set(options QUIET)
    set(oneValueArgs
        PROJ
        PREFIX
        DOWNLOAD_DIR
        SOURCE_DIR
        BINARY_DIR
        CONFIGURE_COMMAND
        BUILD_COMMAND
        INSTALL_COMMAND
        TEST_COMMAND
    )
    set(multiValueArgs "")

    cmake_parse_arguments(DL_ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Hide output if requested
    if (DL_ARGS_QUIET)
        set(OUTPUT_QUIET "OUTPUT_QUIET")
    else()
        unset(OUTPUT_QUIET)
        message(STATUS "Downloading/updating ${DL_ARGS_PROJ}")
    endif()

    if (NOT DL_ARGS_PREFIX)
        set(DL_ARGS_PREFIX "${CMAKE_BINARY_DIR}")
    else()
        get_filename_component(DL_ARGS_PREFIX "${DL_ARGS_PREFIX}" ABSOLUTE
                               BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
    if (NOT DL_ARGS_DOWNLOAD_DIR)
        set(DL_ARGS_DOWNLOAD_DIR "${DL_ARGS_PREFIX}/${DL_ARGS_PROJ}-download")
    endif()

    if (NOT DL_ARGS_SOURCE_DIR)
        set(DL_ARGS_SOURCE_DIR "${DL_ARGS_PREFIX}/${DL_ARGS_PROJ}-src")
    endif()
    if (NOT DL_ARGS_BINARY_DIR)
        set(DL_ARGS_BINARY_DIR "${DL_ARGS_PREFIX}/${DL_ARGS_PROJ}-build")
    endif()
    set(${DL_ARGS_PROJ}_SOURCE_DIR "${DL_ARGS_SOURCE_DIR}" PARENT_SCOPE)
    set(${DL_ARGS_PROJ}_BINARY_DIR "${DL_ARGS_BINARY_DIR}" PARENT_SCOPE)

    file(REMOVE "${DL_ARGS_DOWNLOAD_DIR}/CMakeCache.txt")

    configure_file("${_DownloadProjectDir}/DownloadProject.CMakeLists.cmake.in"
                   "${DL_ARGS_DOWNLOAD_DIR}/CMakeLists.txt")
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
                        -D "CMAKE_MAKE_PROGRAM:FILE=${CMAKE_MAKE_PROGRAM}"
                        .
                    RESULT_VARIABLE result
                    ${OUTPUT_QUIET}
                    WORKING_DIRECTORY "${DL_ARGS_DOWNLOAD_DIR}"
    )
    if(result)
        message(FATAL_ERROR "CMake step for ${DL_ARGS_PROJ} failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
                    RESULT_VARIABLE result
                    ${OUTPUT_QUIET}
                    WORKING_DIRECTORY "${DL_ARGS_DOWNLOAD_DIR}"
    )
    if(result)
        message(FATAL_ERROR "Build step for ${DL_ARGS_PROJ} failed: ${result}")
    endif()

endfunction()
