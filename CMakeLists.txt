cmake_minimum_required( VERSION 3.4 )

project( axl.gl VERSION 0.37.84 DESCRIPTION "A simple and multiplatform C++ game library." )

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
set( AXLGL_INSTALL_ARCHIVES ${AXLGL_STANDALONE} CACHE BOOL "" )
set( AXLGL_INSTALL_RUNTIMES TRUE CACHE BOOL "" )
set( BUILD_TESTS FALSE CACHE BOOL "" )
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" )
# set( BUILD_SHARED_LIBS ON )

if( BUILD_SHARED_LIBS )
	set( AXLGL_TYPE SHARED )
endif()

set( THIRD_PARTY_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../include" )
set( THIRD_PARTY_LIB_DIR "${PROJECT_SOURCE_DIR}/../lib" )

set( AXLMATH_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../axl.math" CACHE PATH "Path to axl.math library source." )
set( AXLMATH_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLMATH_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
add_subdirectory( "${AXLMATH_SOURCE_DIR}" axl.math )

set( AXLUTIL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../axl.util" CACHE PATH "Path to axl.util library source." )
set( AXLUTIL_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLUTIL_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
add_subdirectory( "${AXLUTIL_SOURCE_DIR}" axl.util )

set( AXLGLFL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../axl.glfl" CACHE PATH "Path to axl.glfl library source." )
set( AXLGLFL_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLGLFL_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
add_subdirectory( "${AXLGLFL_SOURCE_DIR}" axl.glfl )

add_subdirectory( src "axl.gl" )

if( AXLGL_BUILD_TESTS )
	add_subdirectory( tests "axl.gl.tests" )
endif()
