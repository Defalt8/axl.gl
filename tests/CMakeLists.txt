
# set( CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib" )
link_libraries( axl.gl gdi32.lib user32.lib )

add_executable( axl.gl.lib "lib.cpp")
add_executable( axl.gl.display "Display.cpp")
add_executable( axl.gl.base "Base.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.view "View.cpp" "common/GLC.cpp" )
add_executable( axl.gl.texture3d "Texture3D.cpp" "common/GLC.cpp" )
add_executable( axl.gl.renderbuffer "RenderBuffer.cpp" "common/GLC.cpp" )
add_executable( axl.gl.rendertotexture "RenderToTexture.cpp" "common/GLC.cpp" )
add_executable( axl.gl.projection "Projection.cpp" "common/GLC.cpp" )
add_executable( axl.gl.camera3d "Camera3D.cpp" "common/GLC.cpp" )
add_executable( axl.gl.shader "Shader.cpp" "common/GLC.cpp" )
add_executable( axl.gl.font "Font.cpp" "common/GLC.cpp" )
add_executable( axl.gl.text "Text.cpp" "common/GL.cpp" )
add_executable( axl.gl.uielement "UIElement.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.uicontainer "UIContainer.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.uibutton "UIButton.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.uitextview "UITextView.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.uitextinput "UITextInput.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.uislider "UISlider.cpp" "common/GL.cpp" "common/MainView.cpp" )
add_executable( axl.gl.viewports "Viewports.cpp" "common/GL.cpp" "common/MainView.cpp" )

set_target_properties( axl.gl.lib
	axl.gl.display
	axl.gl.base
	axl.gl.view
	axl.gl.texture3d
	axl.gl.renderbuffer
	axl.gl.rendertotexture
	axl.gl.projection
	axl.gl.camera3d
	axl.gl.shader
	axl.gl.font
	axl.gl.text
	axl.gl.uielement
	axl.gl.uicontainer
	axl.gl.uibutton
	axl.gl.uitextview
	axl.gl.uitextinput
	axl.gl.uislider
	axl.gl.viewports
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AXLGL_RUNTIME_DIR}"
)
