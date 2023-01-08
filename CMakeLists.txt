cmake_minimum_required(VERSION "3.9")
project("metamod-p" LANGUAGES "CXX")

include("CTest")
enable_testing()

list(APPEND "CMAKE_MODULE_PATH" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include("_inspect_platform")
_inspect_platform()

add_subdirectory("metamod")
add_subdirectory("stub_plugin")
add_subdirectory("trace_plugin")
add_subdirectory("wdmisc_plugin")

add_subdirectory("tests")

include("_print_todo_warnings")
_print_todo_warnings()
