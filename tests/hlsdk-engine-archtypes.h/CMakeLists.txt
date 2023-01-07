unset("_name" CACHE)
unset("_command" CACHE)

mark_as_advanced("_name")
mark_as_advanced("_command")

get_filename_component("_name" "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set("_name" "tests.${_name}")

add_executable("${_name}" "main.cxx")

_initialize_final_target("${_name}")

set_property(TARGET "${_name}" PROPERTY "PREFIX")
set_property(TARGET "${_name}" PROPERTY "OUTPUT_NAME" "executable")
set_property(TARGET "${_name}" PROPERTY "INTERPROCEDURAL_OPTIMIZATION")

target_include_directories("${_name}" PRIVATE "${CMAKE_SOURCE_DIR}/hlsdk/engine")

if(CMAKE_CROSSCOMPILING)
    if(":${CMAKE_CROSSCOMPILING_EMULATOR}:" STREQUAL "::")
        message(WARNING "`${_name}` test disabled because `CMAKE_CROSSCOMPILING` enabled but `CMAKE_CROSSCOMPILING_EMULATOR` not specified")
    else()
        list(APPEND "_command" "${CMAKE_CROSSCOMPILING_EMULATOR}")
        list(APPEND "_command" "$<TARGET_FILE:${_name}>")
    endif()
else()
    list(APPEND "_command" "$<TARGET_FILE:${_name}>")
endif()

if(NOT ":${_command}:" STREQUAL "::")
    add_test(NAME "${_name}" COMMAND ${_command})
endif()
