include("CheckIPOSupported")

function("_initialize_final_target" "_target_name")
    unset("_ipo_supported" CACHE)

    mark_as_advanced("_ipo_supported")

    if(":${_target_name}:" STREQUAL "::")
        message(FATAL_ERROR "target_name is empty")
    endif()

    if("cxx_std_98" IN_LIST "CMAKE_CXX_COMPILE_FEATURES")
        target_compile_features("${_target_name}" PRIVATE "cxx_std_98")
    endif()

    check_ipo_supported(RESULT "_ipo_supported")

    set_target_properties("${_target_name}" PROPERTIES
        "PREFIX" ""
        "OUTPUT_NAME" "${_target_name}"
        "CXX_STANDARD" "98"
        "C_VISIBILITY_PRESET" "hidden"
        "CXX_VISIBILITY_PRESET" "hidden"
        "VISIBILITY_INLINES_HIDDEN" TRUE
        "POSITION_INDEPENDENT_CODE" TRUE
        "INTERPROCEDURAL_OPTIMIZATION" "${_ipo_supported}"
    )

    target_compile_definitions("${_target_name}" PRIVATE "$<$<CONFIG:Release>:__INTERNALS_USE_REGPARAMS__>")
    target_compile_definitions("${_target_name}" PRIVATE "$<$<CONFIG:Release>:OPT_TYPE=\"optimized+meta_debug-disabled\">")
    target_compile_definitions("${_target_name}" PRIVATE "$<$<CONFIG:Debug>:OPT_TYPE=\"debug\">")    

    if (MSVC)
        target_compile_options("${_target_name}" PRIVATE "/arch:sse" "/arch:sse2")
        target_compile_options("${_target_name}" PRIVATE "/GR-" "/EH-")
    else()
        target_compile_options("${_target_name}" PRIVATE "-Wno-write-strings")
        target_compile_options("${_target_name}" PRIVATE "-mtune=generic" "-msse" "-msse2")
        target_compile_options("${_target_name}" PRIVATE "-fno-rtti" "-fno-exceptions")
        target_compile_options("${_target_name}" PRIVATE "-fomit-frame-pointer" "-funsafe-math-optimizations")
        target_link_options("${_target_name}" PRIVATE "-static-libgcc" "-static-libstdc++" "-Wl,--no-undefined")
        if(WIN32)
            target_link_options("${_target_name}" PRIVATE "--add-stdcall-alias")
        endif()
    endif()
endfunction()
