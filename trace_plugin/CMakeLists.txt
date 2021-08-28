include("_initialize_final_target")

add_library("trace_plugin" MODULE)

target_sources("trace_plugin" PRIVATE
    "api_info.cpp" "dllapi.cpp" "dllapi_post.cpp" "engine_api.cpp"
    "engine_api_post.cpp" "h_export.cpp" "log_plugin.cpp" "meta_api.cpp"
    "plugin.cpp" "sdk_util.cpp" "trace_api.cpp" "vdate.cpp"
    "res_meta.rc"
)

_initialize_final_target("trace_plugin")

set_target_properties("trace_plugin" PROPERTIES
    OUTPUT_NAME "trace_mm"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.output/addons/metamod/plugins"
)

target_include_directories("trace_plugin" PRIVATE "../metamod" "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")
