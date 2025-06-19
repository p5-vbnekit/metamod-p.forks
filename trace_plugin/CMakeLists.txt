add_library("trace_plugin" MODULE)

target_sources("trace_plugin" PRIVATE
    "api_info.cpp" "dllapi.cpp" "dllapi_post.cpp" "engine_api.cpp"
    "engine_api_post.cpp" "h_export.cpp" "log_plugin.cpp" "meta_api.cpp"
    "plugin.cpp" "sdk_util.cpp" "trace_api.cpp" "vdate.cpp"
    "res_meta.rc"
)

include("_initialize_final_target")
_initialize_final_target("trace_plugin")

set_target_properties("trace_plugin" PROPERTIES
    OUTPUT_NAME "trace_mm"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.output/addons/metamod/plugins"
)

target_include_directories("trace_plugin" PRIVATE "../metamod" "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")

if(NOT MSVC)
    unset("_platform.hidden_static" CACHE)
    get_property("_platform.hidden_static" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_inspect_platform.hidden_static")
    mark_as_advanced("_platform.hidden_static")
    if(NOT "${_platform.hidden_static}")
        set_property(SOURCE "api_info.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "DO_NOT_USE_DLLHIDDEN_WITH_STATIC_LINKAGE=1")
    endif()
    if(WIN32)
        unset("_platform.windres_sz_definition" CACHE)
        get_property("_platform.windres_sz_definition" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_inspect_platform.windres_sz_definition")
        mark_as_advanced("_platform.windres_sz_definition")
        if(NOT "${_platform.windres_sz_definition}")
            # quotes in definitions aren't re-escaped properly by `windres` when passing definitions to preprocessor
            # check this bug here: https://sourceware.org/bugzilla/show_bug.cgi?id=27843
            # work around this: redefining `OPT_TYPE` (ugly and dirty hack)
            set_property(SOURCE "res_meta.rc" APPEND PROPERTY COMPILE_DEFINITIONS "$<$<CONFIG:Debug>:OPT_TYPE=\\\"debug\\\">")
            set_property(SOURCE "res_meta.rc" APPEND PROPERTY COMPILE_DEFINITIONS "$<$<CONFIG:Release>:OPT_TYPE=\\\"optimized+meta_debug-disabled\\\">")
        endif()
    endif()
endif()
