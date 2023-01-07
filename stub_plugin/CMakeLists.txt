add_library("stub_plugin" MODULE)

target_sources("stub_plugin" PRIVATE
    "dllapi.cpp" "engine_api.cpp" "h_export.cpp" "meta_api.cpp" "sdk_util.cpp"
)

include("_initialize_final_target")
_initialize_final_target("stub_plugin")

set_target_properties("stub_plugin" PROPERTIES
    OUTPUT_NAME "stub_mm"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.output/addons/metamod/plugins"
)

target_include_directories("stub_plugin" PRIVATE "../metamod" "../hlsdk/dlls" "../hlsdk/engine" "../hlsdk/common")
