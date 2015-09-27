file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.h *.hpp)

add_executable(lake ${src_files})

target_compile_features(lake PUBLIC cxx_strong_enums)
