cmake_minimum_required(VERSION "3.9")
project("metamod-p.compile_time_tests.windres_sz_definition" LANGUAGES "CXX")
add_executable("executable" "main.cxx" "main.rc")
set_property(SOURCE "main.rc" APPEND PROPERTY COMPILE_DEFINITIONS "INTERNAL_NAME_VALUE=\"EXECUTABLE.EXE\"")
