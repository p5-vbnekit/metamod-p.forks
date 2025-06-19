function("_inspect_platform")
    unset("_state" CACHE)
    get_property("_state" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_inspect_platform.state" SET)
    mark_as_advanced("_state")

    if(_state)
        return()
    endif()

    set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_inspect_platform.state" TRUE)

    unset("_ipo" CACHE)
    unset("_hidden_static" CACHE)
    unset("_stdcall_alias" CACHE)
    unset("_windres_sz_definition" CACHE)

    set("_ipo" FALSE)
    set("_hidden_static" FALSE)
    set("_stdcall_alias" FALSE)
    set("_windres_sz_definition" FALSE)

    mark_as_advanced("_ipo")
    mark_as_advanced("_hidden_static")
    mark_as_advanced("_stdcall_alias")
    mark_as_advanced("_windres_sz_definition")

    include("CheckIPOSupported")
    check_ipo_supported(RESULT "_ipo")

    if(_ipo)
        include("_check_ipo_with_static_libstdcxx")
        _check_ipo_with_static_libstdcxx("_ipo")
        if(NOT (_ipo OR _state))
            message(WARNING "current toolchain doesn't support interprocedural optimization with `-static-libstdc++` because standard c++ library is bogus")
        endif()
    elseif(NOT _state)
        message(WARNING "current toolchain doesn't support interprocedural optimization")
    endif()

    if(WIN32 AND (NOT MSVC))
        include("_check_hidden_static")
        _check_hidden_static("_hidden_static")
        if(NOT _hidden_static)
            message(WARNING "current toolchain ignores \"hidden\" visibility attribute for static linkage")
        endif()
        include("_check_stdcall_alias")
        _check_stdcall_alias("_stdcall_alias")
        if(NOT _stdcall_alias)
            message(WARNING "current toolchain linker doesn't support `-Wl,--add-stdcall-alias`")
        endif()
        include("_check_windres_sz_definition")
        _check_windres_sz_definition("_windres_sz_definition")
        if(NOT _windres_sz_definition)
            message(WARNING "current toolchainchain `windres` doesn't properly support quoted definitions: -DTEXT=\\\"TEXT\\\"")
        endif()
    endif()

    set_property(
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        PROPERTY "_inspect_platform.ipo"
        "${_ipo}"
    )

    set_property(
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        PROPERTY "_inspect_platform.hidden_static"
        "${_hidden_static}"
    )

    set_property(
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        PROPERTY "_inspect_platform.stdcall_alias"
        "${_stdcall_alias}"
    )

    set_property(
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        PROPERTY "_inspect_platform.windres_sz_definition"
        "${_windres_sz_definition}"
    )
endfunction()
