include("_initialize_final_target")

unset("_compile_time" CACHE)
unset("_compile_date" CACHE)

mark_as_advanced("_compile_time")
mark_as_advanced("_compile_date")

add_library("wdmisc_plugin" MODULE)

target_sources("wdmisc_plugin" PRIVATE
    "dll_plugin.cpp" "engine_api.cpp" "h_export.cpp" "log_plugin.cpp"
    "meta_api.cpp" "sdk_util.cpp" "vdate.cpp"
    "wdmisc.cpp"
)

_initialize_final_target("wdmisc_plugin")

set_target_properties("wdmisc_plugin" PROPERTIES
    OUTPUT_NAME "wdmisc_mm"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.output/addons/metamod/plugins"
)

string(TIMESTAMP "_compile_time" "%H:%M:%S" UTC)
string(TIMESTAMP "_compile_date" "%b %d %Y" UTC)

set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "COMPILE_TZ=\"UTC\"")
set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "METAMOD_COMPILE_TIME=\"${_compile_time}\"")
set_property(SOURCE "vdate.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "METAMOD_COMPILE_DATE=\"${_compile_date}\"")

target_include_directories("wdmisc_plugin" PRIVATE "../metamod" "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")
