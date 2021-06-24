cmake_minimum_required( VERSION 3.4 )

project( axl.gl VERSION 0.59.203 DESCRIPTION "A cross-platform C++ graphics library." )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
	set( AXLGL_STANDALONE TRUE )
else()
	set( AXLGL_STANDALONE FALSE )
endif()

set( CMAKE_CXX_STANDARD 11 CACHE STRING "C++ compiler standard.")
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL "" )
set( CMAKE_BUILD_TYPE Debug CACHE STRING "Build configuration type: ${CMAKE_CONFIGURATION_TYPES}." )
set( AXLGL_TYPE STATIC CACHE STRING "" )
set( AXLGL_RUNTIME_DIR "${PROJECT_BINARY_DIR}/bin" CACHE PATH "Path to build ruintime objects like shared library." )
set( AXLGL_ARCHIVE_DIR "${PROJECT_BINARY_DIR}/lib" CACHE PATH "Path to build ruintime objects like shared library." )
set( AXLGL_INSTALL_HEADERS ${AXLGL_STANDALONE} CACHE BOOL "" )
set( AXLGL_BUILD_TESTS ${AXLGL_STANDALONE} CACHE BOOL "" )
set( AXLGL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "axl.gl headers path." )

set( THIRD_PARTY_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../include" )
set( THIRD_PARTY_LIB_DIR "${CMAKE_SOURCE_DIR}/../lib" )

set( AXLMATH_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies/axl.math" CACHE PATH "Path to axl.math library source." )
set( AXLUTIL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies/axl.util" CACHE PATH "Path to axl.util library source." )
set( AXLGLFL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies/axl.glfl" CACHE PATH "Path to axl.glfl library source." )
set( AXLMEDIA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies/axl.media" CACHE PATH "Path to axl.media library source." )
set( AXLMATH_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLMATH_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
set( AXLUTIL_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLUTIL_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
set( AXLGLFL_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLGLFL_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )
set( AXLMEDIA_ARCHIVE_DIR "${AXLGL_ARCHIVE_DIR}" )
set( AXLMEDIA_RUNTIME_DIR "${AXLGL_RUNTIME_DIR}" )

add_subdirectory( "${AXLMATH_SOURCE_DIR}" axl.math )
add_subdirectory( "${AXLUTIL_SOURCE_DIR}" axl.util )
add_subdirectory( "${AXLGLFL_SOURCE_DIR}" axl.glfl )
add_subdirectory( "${AXLMEDIA_SOURCE_DIR}" axl.media )

add_subdirectory( src axl.gl )

if( AXLGL_BUILD_TESTS )
	add_subdirectory( tests "axl.gl.tests" )
endif()

if( AXLGL_INSTALL_HEADERS )
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/everything.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/lib.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/platform.h"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Display.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Application.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/View.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Context.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/ContextObject.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/inputs.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projections.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/cameras.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx.hpp"
		DESTINATION "include/axl.gl"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/KeyCodes.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Keyboard.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Mouse.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Touch.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Key.hpp"
		DESTINATION "include/axl.gl/input"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Projection.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Orthographic.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Perspective.hpp"
		DESTINATION "include/axl.gl/projection"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/camera/Camera3D.hpp"
		DESTINATION "include/axl.gl/camera"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/textures.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffers.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shaders.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI.hpp"
		DESTINATION "include/axl.gl/gfx"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture1D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture2D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture3D.hpp"
		DESTINATION "include/axl.gl/gfx/texture"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/FrameBuffer.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/RenderBuffer.hpp"
		DESTINATION "include/axl.gl/gfx/buffer"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shader/Shader.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shader/Program.hpp"
		DESTINATION "include/axl.gl/gfx/shader"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text/Font.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text/Text.hpp"
		DESTINATION "include/axl.gl/gfx/text"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Component.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Container.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Element.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Layout.hpp"
		DESTINATION "include/axl.gl/gfx/UI"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/Button.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/TextView.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/TextInput.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/Slider.hpp"
		DESTINATION "include/axl.gl/gfx/UI/elements"
	)
	install( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Linear.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Split.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Grid.hpp"
		DESTINATION "include/axl.gl/gfx/UI/layouts"
	)
endif()
