set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE )
set( DEBUG_SUFFIX "" )
set( TYPE_SUFFIX "" )

if( "${AXLGL_TYPE}" MATCHES "^[Ss][Hh][Aa][Rr][Ee][Dd]$" OR BUILD_SHARED_LIBS )
	set( AXLGL_TYPE SHARED )
else()
	set( AXLGL_TYPE STATIC )
	set( TYPE_SUFFIX s )
endif()

if( "${CMAKE_BUILD_TYPE}" MATCHES "^[Dd][Ee][Bb][Uu][Gg]$" )
	set( DEBUG_SUFFIX d )
endif()

set( LIBFREETYPE "${THIRD_PARTY_LIB_DIR}/freetype${DEBUG_SUFFIX}.lib" )


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
	"gfx/shader/Shader.cpp"
	"gfx/shader/Program.cpp"
	"gfx/text/Font.cpp"
	"gfx/text/Text.cpp"
	"gfx/UI/Component.cpp"
	"gfx/UI/Container.cpp"
	"gfx/UI/Element.cpp"
	"gfx/UI/Layout.cpp"
	"gfx/UI/layouts/Linear.cpp"
	"gfx/UI/layouts/Split.cpp"
	"gfx/UI/layouts/Grid.cpp"
	"gfx/UI/elements/Button.cpp"
	"gfx/UI/elements/TextView.cpp"
	"gfx/UI/elements/TextInput.cpp"
	"gfx/UI/elements/Slider.cpp"

	"${AXLGL_INCLUDE_DIR}/axl.gl/everything.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/lib.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/platform.h"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Display.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Application.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/View.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Context.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/ContextObject.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/KeyCodes.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Keyboard.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Mouse.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Touch.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/input/Key.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/inputs.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Projection.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Orthographic.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projection/Perspective.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/projections.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/camera/Camera3D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/cameras.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture1D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture2D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/texture/Texture3D.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/textures.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/FrameBuffer.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffer/RenderBuffer.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/buffers.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shader/Shader.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shader/Program.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/shaders.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text/Font.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text/Text.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/text.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Component.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Container.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Element.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/Layout.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Linear.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Split.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/layouts/Grid.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/Button.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/TextView.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/TextInput.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/gfx/UI/elements/Slider.hpp"
)
target_include_directories( axl.gl
	PUBLIC "${AXLGL_INCLUDE_DIR}"
	PUBLIC "${THIRD_PARTY_INCLUDE_DIR}"
)
target_link_libraries( axl.gl
	PUBLIC axl.math
	PUBLIC axl.util
	PUBLIC axl.glfl
	PUBLIC ${LIBFREETYPE}
)
target_compile_definitions( axl.gl
	PRIVATE AXLGL_BUILD
	PRIVATE AXLGL_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
	PRIVATE AXLGL_VERSION_MINOR=${PROJECT_VERSION_MINOR}
	PRIVATE AXLGL_VERSION_PATCH=${PROJECT_VERSION_PATCH}
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

set_target_properties( axl.gl PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY "${AXLGL_ARCHIVE_DIR}"
	RUNTIME_OUTPUT_NAME "axl.gl${TYPE_SUFFIX}${DEBUG_SUFFIX}"
	ARCHIVE_OUTPUT_NAME "axl.gl${TYPE_SUFFIX}${DEBUG_SUFFIX}"
)

install( TARGETS axl.gl
	ARCHIVE DESTINATION "lib"
	RUNTIME DESTINATION "bin"
)
