
add_executable( axl.gl.lib "lib.cpp" )

set_target_properties( axl.gl.lib
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
)
target_link_libraries( axl.gl.lib PRIVATE axl.gl )