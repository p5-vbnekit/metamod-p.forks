cmake_minimum_required(VERSION "3.9")
project("metamod-p.compile_time_tests.hidden_static" LANGUAGES "CXX")
add_executable("executable" "main.cxx")
target_compile_options("executable" PRIVATE "-Werror=attributes")
