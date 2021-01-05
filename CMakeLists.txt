cmake_minimum_required( VERSION 3.4 )

project( axl.gl VERSION 0.1.0 DESCRIPTION "A simple and multiplatform C++ game library." LANGUAGES CXX )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
	set( AXLGL_STANDALONE TRUE )
else()
	set( AXLGL_STANDALONE FALSE )
endif()

set( AXLGL_TYPE STATIC CACHE STRING "" )
set( AXLGL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "axl.gl headers path." )
set( AXLGL_RUNTIME_DIR "${PROJECT_BINARY_DIR}/bin" CACHE PATH "Path to build ruintime objects like shared library." )
set( AXLGL_ARCHIVE_DIR "${PROJECT_BINARY_DIR}/lib" CACHE PATH "Path to build ruintime objects like shared library." )
set( AXLGL_BUILD_TESTS ${AXLGL_STANDALONE} CACHE BOOL "" )
set( AXLGL_INSTALL_HEADERS ${AXLGL_STANDALONE} CACHE BOOL "" )
set( AXLGL_INSTALL_LIBRARY ${AXLGL_STANDALONE} CACHE BOOL "" )
set( BUILD_TESTS FALSE CACHE BOOL "" )
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" )

if( BUILD_SHARED_LIBS )
	set( AXLGL_TYPE SHARED )
endif()

include_directories( ${AXLGL_INCLUDE_DIR} )

add_subdirectory( src )
if( BUILD_TESTS AND AXLGL_BUILD_TESTS )
	add_subdirectory( tests )
endif()
