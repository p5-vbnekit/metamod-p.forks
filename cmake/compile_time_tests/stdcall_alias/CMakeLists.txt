cmake_minimum_required(VERSION "3.10")
project("metamod-p.compile_time_tests.stdcall_alias" LANGUAGES "CXX")
add_executable("executable" "main.cxx")
target_link_options("executable" PRIVATE "-Wl,--add-stdcall-alias")
