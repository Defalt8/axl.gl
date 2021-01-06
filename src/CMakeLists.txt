
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

	"${AXLGL_INCLUDE_DIR}/axl.gl/lib.hpp"
	"${AXLGL_INCLUDE_DIR}/axl.gl/Display.hpp"
)
target_include_directories( axl.gl PUBLIC "${AXLGL_INCLUDE_DIR}" )
target_link_libraries( axl.gl PUBLIC axl.math )
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
		DESTINATION "include/axl.gl"
	)
endif()
