function("_check_hidden_static" "_output_variable")
    if(":${_output_variable}:" STREQUAL "::")
        message(FATAL_ERROR "_output_variable is empty")
    endif()

    unset("_result" CACHE)
    get_property("_result" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_check_hidden_static.result" SET)
    mark_as_advanced("_result")

    if (_result)
        get_property("_result" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_check_hidden_static.result")
        if(":${_result}:" STREQUAL "::")
            message(FATAL_ERROR "${CMAKE_SOURCE_DIR}: _check_hidden_static.result property is empty")
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
            "${_compile_time_tests_directories.build}/hidden_static"
            "${_compile_time_tests_directories.source}/hidden_static"
            "metamod-p.compile_time_tests.hidden_static"
        )

        set_property(
            DIRECTORY "${CMAKE_SOURCE_DIR}"
            PROPERTY "_check_hidden_static.result"
            "${_result}"
        )
    endif()

    set("${_output_variable}" "${_result}" PARENT_SCOPE)
endfunction()
