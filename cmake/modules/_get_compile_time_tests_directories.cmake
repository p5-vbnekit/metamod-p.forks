function("_get_compile_time_tests_directories" "_output_prefix")
    if(":${_output_prefix}:" STREQUAL "::")
        message(FATAL_ERROR "output_prefix is empty")
    endif()

    unset("_state" CACHE)
    unset("_build" CACHE)
    unset("_source" CACHE)

    set("_state" FALSE)
    set("_build" "")
    set("_source" "")

    mark_as_advanced("_state")
    mark_as_advanced("_build")
    mark_as_advanced("_source")

    get_property(
        "_state"
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        PROPERTY "_get_compile_time_tests_directories.state"
        SET
    )

    if(_state)
        get_property(
            "_build"
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_get_compile_time_tests_directories.build"
        )

        get_property(
            "_source"
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_get_compile_time_tests_directories.source"
        )

        if(":${_build}:" STREQUAL "::")
            message(FATAL_ERROR "${CMAKE_SOURCE_DIR}: _get_compile_time_tests_directories.build property is empty")
        endif()

        if(":${_source}:" STREQUAL "::")
            message(FATAL_ERROR "${CMAKE_SOURCE_DIR}: _get_compile_time_tests_directories.source property is empty")
        endif()
    else()
        set_property(
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_get_compile_time_tests_directories.state"
            TRUE
        )

        set("_build" "${CMAKE_BINARY_DIR}/compile_time_tests")
        get_filename_component("_build" "${_build}" ABSOLUTE)

        set("_source" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
        if(":${_source}:" STREQUAL "::")
            set("_source" "${CMAKE_SOURCE_DIR}/cmake")
        else()
            set("_source" "${_source}/..")
        endif()
        set("_source" "${_source}/compile_time_tests")
        get_filename_component("_source" "${_source}" ABSOLUTE)

        set_property(
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_get_compile_time_tests_directories.build"
            "${_build}"
        )

        set_property(
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_get_compile_time_tests_directories.source"
            "${_source}"
        )
    endif()

    set("${_output_prefix}build" "${_build}" PARENT_SCOPE)
    set("${_output_prefix}source" "${_source}" PARENT_SCOPE)
endfunction()
