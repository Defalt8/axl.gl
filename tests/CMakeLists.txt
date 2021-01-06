
link_libraries( axl.gl gdi32.lib user32.lib )

add_executable( axl.gl.lib "lib.cpp")
add_executable( axl.gl.display "Display.cpp")

set_target_properties( axl.gl.lib axl.gl.display
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
)