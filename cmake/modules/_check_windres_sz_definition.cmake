function("_check_windres_sz_definition" "_output_variable")
    if(":${_output_variable}:" STREQUAL "::")
        message(FATAL_ERROR "_output_variable is empty")
    endif()

    unset("_result" CACHE)
    get_property("_result" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_check_windres_sz_definition.result" SET)
    mark_as_advanced("_result")

    if (_result)
        get_property("_result" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_check_windres_sz_definition.result")
        if(":${_result}:" STREQUAL "::")
            message(FATAL_ERROR "${CMAKE_SOURCE_DIR}: _check_windres_sz_definition.result property is empty")
        endif()
    else()
        unset("_compile_time_tests_directories.build" CACHE)
        unset("_compile_time_tests_directories.source" CACHE)
        set("_compile_time_tests_directories.build" "")
        set("_compile_time_tests_directories.source" "")
        mark_as_advanced("_compile_time_tests_directories.build")
        mark_as_advanced("_compile_time_tests_directories.source")
        include("_get_compile_time_tests_directories")
        _get_compile_time_tests_directories("_compile_time_tests_directories.")

        try_compile(
            "_result"
            "${_compile_time_tests_directories.build}/windres_sz_definition"
            "${_compile_time_tests_directories.source}/windres_sz_definition"
            "metamod-p.compile_time_tests.windres_sz_definition"
        )

        set_property(
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_check_windres_sz_definition.result"
            "${_result}"
        )
    endif()

    set("${_output_variable}" "${_result}" PARENT_SCOPE)
endfunction()
