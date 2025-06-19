unset("_compile_time" CACHE)
unset("_compile_date" CACHE)

mark_as_advanced("_compile_time")
mark_as_advanced("_compile_date")

add_library("metamod" MODULE)

target_sources("metamod" PRIVATE
    "api_hook.cpp" "api_info.cpp" "commands_meta.cpp" "conf_meta.cpp"
    "dllapi.cpp" "engine_api.cpp" "engineinfo.cpp" "game_support.cpp"
    "game_autodetect.cpp" "h_export.cpp" "linkplug.cpp"
    "log_meta.cpp" "meta_eiface.cpp" "metamod.cpp" "mlist.cpp" "mplayer.cpp"
    "mplugin.cpp" "mreg.cpp" "mutil.cpp" "osdep.cpp"
    "osdep_p.cpp" "reg_support.cpp" "sdk_util.cpp"
    "support_meta.cpp" "vdate.cpp"
    "res_meta.rc"
)

include("_initialize_final_target")
_initialize_final_target("metamod")

set_target_properties("metamod" PROPERTIES
    DEFINE_SYMBOL "__METAMOD_BUILD__"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.output/addons/metamod"
)

string(TIMESTAMP "_compile_time" "%H:%M:%S" UTC)
string(TIMESTAMP "_compile_date" "%b %d %Y" UTC)

set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "COMPILE_TZ=\"UTC\"")
set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "METAMOD_COMPILE_TIME=\"${_compile_time}\"")
set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "METAMOD_COMPILE_DATE=\"${_compile_date}\"")

target_include_directories("metamod" PRIVATE "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")

target_compile_definitions("metamod" PRIVATE "$<$<CONFIG:Release>:__BUILD_FAST_METAMOD__>")

if(WIN32)
    target_sources("metamod" PRIVATE "osdep_linkent_win32.cpp" "osdep_detect_gamedll_win32.cpp")
    if(NOT MSVC)
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            target_link_options("metamod" PRIVATE "-Xlinker" "--script" "-Xlinker" "${CMAKE_CURRENT_SOURCE_DIR}/i386pe.merge")
        endif()
        unset("_platform.windres_sz_definition" CACHE)
        get_property("_platform.windres_sz_definition" DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY "_inspect_platform.windres_sz_definition")
        mark_as_advanced("_platform.windres_sz_definition")
        if(NOT "${_platform.windres_sz_definition}")
            message(WARNING
                "quotes in definitions aren't re-escaped properly by `windres` when passing definitions to preprocessor\n"
                "check this bug here: https://sourceware.org/bugzilla/show_bug.cgi?id=27843\n"
                "work around this: redefining `OPT_TYPE` (ugly and dirty hack)"
            )
            set_property(SOURCE "res_meta.rc" APPEND PROPERTY COMPILE_DEFINITIONS "$<$<CONFIG:Debug>:OPT_TYPE=\\\"debug\\\">")
            set_property(SOURCE "res_meta.rc" APPEND PROPERTY COMPILE_DEFINITIONS "$<$<CONFIG:Release>:OPT_TYPE=\\\"optimized+meta_debug-disabled\\\">")
        endif()
    endif()
elseif(UNIX)
    target_link_libraries("metamod" PRIVATE "dl")
    target_sources("metamod" PRIVATE "osdep_linkent_linux.cpp" "osdep_detect_gamedll_linux.cpp")
endif()
