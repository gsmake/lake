file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.h *.hpp)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)

add_executable(lake ${src_files})

target_compile_features(lake PUBLIC cxx_strong_enums)

target_link_libraries(lake lemoon)

set_target_properties(
    lake
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/)
