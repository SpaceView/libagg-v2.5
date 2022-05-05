cmake_minimum_required(VERSION 2.8)

if(NOT WIN32)
	message(FATAL_ERROR "No Supported Platform!")
endif()

SET(PACKAGE libagg)
PROJECT(${PACKAGE})

#FILE(GLOB_RECURSE INC_LIST "include/*.h") 
#FILE(GLOB_RECURSE SRC_LIST "SRC/*.cpp") 
aux_source_directory("${CMAKE_SOURCE_DIR}/include" agg_inc_list)
aux_source_directory("${CMAKE_SOURCE_DIR}/src" agg_src_list)

SET(AGG_SRC
	${agg_inc_list}
	${agg_src_list}
)
include_directories(${CMAKE_SOURCE_DIR}/include)

function(WITH_MODULE argc)
	set(_module ${ARGV0})
	
	if (BUILD_WITH_${_module})
		message(STATUS "Build with ${_module}")
		aux_source_directory("${CMAKE_SOURCE_DIR}/${_module}" src_${_module})
		include_directories(${CMAKE_SOURCE_DIR}/${_module})
		set(AGG_SRC ${AGG_SRC} ${src_${_module}})
	endif()
endfunction()

#option(BUILD_WITH_font_freetype "build with freetype font" OFF)
#WITH_MODULE(font_freetype)

option(BUILD_WITH_font_win32_tt "build with win truetype font" ON)
WITH_MODULE(font_win32_tt)

option(BUILD_WITH_gpc "build with gpc" ON)
WITH_MODULE(gpc)

ADD_LIBRARY(${PACKAGE} STATIC ${AGG_SRC})

aux_source_directory("${CMAKE_SOURCE_DIR}/src/ctrl/" agg_ctrl_src)
aux_source_directory("${CMAKE_SOURCE_DIR}/src/platform/win32/" agg_platform_src)
aux_source_directory("${CMAKE_SOURCE_DIR}/font_win32_tt/" agg_font_win32_tt_src)
aux_source_directory("${CMAKE_SOURCE_DIR}/gpc/" agg_gpc_src)

function(WITH_DEMO argc)
	set(_demo ${ARGV0})
	
	#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/examples)
	include_directories(${CMAKE_SOURCE_DIR}/include)
	include_directories(${CMAKE_SOURCE_DIR}/src)
	#include_directories(${CMAKE_SOURCE_DIR}/font_freetype)
	include_directories(${CMAKE_SOURCE_DIR}/font_win32_tt)
	include_directories(${CMAKE_SOURCE_DIR}/gpc)
	#link_directories(${CMAKE_BINARY_DIR})
	
	SET(_demo_src
		${ARGN}
		${agg_ctrl_src}
		${agg_platform_src}
		#${agg_gpc_src}
	)
	
	add_executable(${_demo} WIN32 ${_demo_src})
	target_link_libraries(${_demo} ${PACKAGE})
	add_dependencies(${_demo} ${PACKAGE})
endfunction()

add_subdirectory(examples)
