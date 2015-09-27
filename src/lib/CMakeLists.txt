include (CheckIncludeFiles)

CHECK_INCLUDE_FILES (sys/event.h LEMOON_KQUEUE_H)
CHECK_INCLUDE_FILES (sys/epoll.h LEMOON_HAS_EPOLL_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lemoon/configure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/lemoon/configure.h IMMEDIATE)


file(GLOB_RECURSE src_files *.c *.cpp *.cxx)
file(GLOB_RECURSE header_files *.h *.hpp *.hxx)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)

add_library(lemoon STATIC ${header_files} ${src_files})

foreach(FILE ${header_files})
		get_filename_component(FILE_NAME ${FILE} NAME)
		string(REPLACE ${FILE_NAME} "" DIRECTORY ${FILE})

		file(RELATIVE_PATH DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${DIRECTORY})

		file(TO_NATIVE_PATH "${DIRECTORY}" DIRECTORY)

		source_group("include\\${DIRECTORY}" FILES ${FILE})
endforeach()

foreach(FILE ${src_files})
		get_filename_component(FILE_NAME ${FILE} NAME)
		string(REPLACE ${FILE_NAME} "" DIRECTORY ${FILE})

		file(RELATIVE_PATH DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${DIRECTORY})

		file(TO_NATIVE_PATH "${DIRECTORY}" DIRECTORY)

		source_group("sources\\${DIRECTORY}" FILES ${FILE})
endforeach()

if(WIN32)
	add_definitions(/DLEMOON_BUILD /wd4311 /D_UNICODE)
elseif(APPLE)
	add_definitions(-D_BSD_SOURCE)
else()
	 add_definitions(-D_POSIX_SOURCE)
endif()

target_compile_features(lemoon PUBLIC cxx_strong_enums)
target_compile_features(lemoon PUBLIC cxx_deleted_functions)
