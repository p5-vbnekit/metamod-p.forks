include("_initialize_final_target")

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

if(NOT MSVC)
    set_property(SOURCE "meta_eiface.cpp" APPEND PROPERTY COMPILE_FLAGS "-Wno-implicit-fallthrough")
endif()

target_include_directories("metamod" PRIVATE "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")

target_compile_definitions("metamod" PRIVATE "$<$<CONFIG:Release>:__BUILD_FAST_METAMOD__>")

if(WIN32)
    target_sources("metamod" PRIVATE "osdep_linkent_win32.cpp" "osdep_detect_gamedll_win32.cpp")
    if((NOT MSVC) AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
        target_link_options("metamod" PRIVATE "-Xlinker" "--script" "-Xlinker" "${CMAKE_CURRENT_SOURCE_DIR}/i386pe.merge")
    endif()
elseif(UNIX)
    target_link_libraries("metamod" PRIVATE "dl")
    target_sources("metamod" PRIVATE "osdep_linkent_linux.cpp" "osdep_detect_gamedll_linux.cpp")
endif()
