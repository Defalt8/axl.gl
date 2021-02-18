
link_libraries( axl.gl gdi32.lib user32.lib )

add_executable( axl.gl.lib "lib.cpp")
add_executable( axl.gl.display "Display.cpp")
add_executable( axl.gl.view "View.cpp" "GLC.cpp" )
add_executable( axl.gl.texture3d "Texture3D.cpp" "GLC.cpp" )
add_executable( axl.gl.renderbuffer "RenderBuffer.cpp" "GLC.cpp" )
add_executable( axl.gl.rendertotexture "RenderToTexture.cpp" "GLC.cpp" )
add_executable( axl.gl.projection "Projection.cpp" "GLC.cpp" )
add_executable( axl.gl.camera3d "Camera3D.cpp" "GLC.cpp" )
add_executable( axl.gl.shader "Shader.cpp" "GLC.cpp" )
add_executable( axl.gl.font "Font.cpp" "GLC.cpp" )
add_executable( axl.gl.text "Text.cpp" "GLC.cpp" )

set_target_properties( axl.gl.lib axl.gl.display axl.gl.view axl.gl.texture3d axl.gl.renderbuffer
	axl.gl.rendertotexture axl.gl.projection axl.gl.camera3d axl.gl.shader axl.gl.font axl.gl.text
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
)