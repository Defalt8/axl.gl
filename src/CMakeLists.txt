
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE )
set( MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
set( AXLGL_TYPE STATIC CACHE STRING "" )
if( "${CMAKE_BUILD_TYPE}" MATCHES "^[Dd][Ee][Bb][Uu][Gg]$" )
	set( DEBUG_SUFFIX d )
else()
	set( DEBUG_SUFFIX "" )
endif()
if( "${AXLGL_TYPE}" STREQUAL "SHARED" )
	set( TYPE_SUFFIX "" )
else()
	set( TYPE_SUFFIX s )
endif()

add_library( axl.gl ${AXLGL_TYPE}
	"lib.cpp"
	"Display.cpp"
	"Application.cpp"
	"View.cpp"
	"Context.cpp"
	"ContextObject.cpp"
	"input/Mouse.cpp"
	"input/Keyboard.cpp"
	"input/Touch.cpp"
	"input/Key.cpp"
	"projection/Projection.cpp"
	"projection/Orthographic.cpp"
	"projection/Perspective.cpp"
	"camera/Camera3D.cpp"
	"gfx/texture/Texture.cpp"
	"gfx/texture/Texture1D.cpp"
	"gfx/texture/Texture2D.cpp"
	"gfx/texture/Texture3D.cpp"
	"gfx/buffer/FrameBuffer.cpp"
	"gfx/buffer/RenderBuffer.cpp"

	"${AXLGL_INCLUDE_DIR}/axl.gl/lib.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Display.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Application.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/View.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Context.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/ContextObject.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Mouse.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Keyboard.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Touch.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Key.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projections.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Projection.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Orthographic.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Perspective.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/camera/Camera3D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/textures.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture1D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture2D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture3D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/FrameBuffer.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/RenderBuffer.hpp"
)
target_include_directories( axl.gl PUBLIC "${AXLGL_INCLUDE_DIR}" )
target_link_libraries( axl.gl PUBLIC axl.math axl.util axl.glfl )
set_target_properties( axl.gl PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY "${AXLGL_ARCHIVE_DIR}"
	RUNTIME_OUTPUT_NAME "axl.gl${TYPE_SUFFIX}${DEBUG_SUFFIX}"
	ARCHIVE_OUTPUT_NAME "axl.gl${TYPE_SUFFIX}${DEBUG_SUFFIX}"
)
target_compile_definitions( axl.gl
	PRIVATE AXLGL_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
	PRIVATE AXLGL_VERSION_MINOR=${PROJECT_VERSION_MINOR}
	PRIVATE AXLGL_VERSION_PATCH=${PROJECT_VERSION_PATCH}
	PRIVATE AXLGL_BUILD
)
if( "${AXLGL_TYPE}" STREQUAL "SHARED" )
	target_compile_definitions( axl.gl PUBLIC AXLGL_SHARED )
else()
	target_compile_definitions( axl.gl PUBLIC AXLGL_STATIC )
endif()

if( "${CMAKE_BUILD_TYPE}" MATCHES "[Dd][Ee][Bb][Uu][Gg]" )
	target_compile_definitions( axl.gl PRIVATE DEBUG )
elseif( "${CMAKE_BUILD_TYPE}" MATCHES "[Rr][Ee][Ll]" )
	target_compile_definitions( axl.gl PRIVATE NDEBUG )
endif()

if( AXLGL_INSTALL_ARCHIVES AND AXLGL_INSTALL_RUNTIMES )
	INSTALL( TARGETS axl.gl
		ARCHIVE DESTINATION "lib"
		RUNTIME DESTINATION "bin"
	)
elseif( AXLGL_INSTALL_ARCHIVES )
	INSTALL( TARGETS axl.gl
		ARCHIVE DESTINATION "lib"
	)
elseif( AXLGL_INSTALL_RUNTIMES )
	INSTALL( TARGETS axl.gl
		RUNTIME DESTINATION "bin"
	)
endif()
if( AXLGL_INSTALL_HEADERS )
	INSTALL( FILES
		"${AXLGL_INCLUDE_DIR}/axl.gl/lib.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Display.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Application.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/View.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/Context.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/ContextObject.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Mouse.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Keyboard.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Touch.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/input/Key.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projections.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Projection.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Orthographic.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Perspective.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/camera/Camera3D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/textures.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture1D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture2D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture3D.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/FrameBuffer.hpp"
		"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/RenderBuffer.hpp"
		DESTINATION "include/axl.gl"
	)
endif()
