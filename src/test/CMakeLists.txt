file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.h *.hpp)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)

add_executable(gsmake-test ${src_files})

target_link_libraries(gsmake-test lemoon)

set_target_properties(
        gsmake-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/)