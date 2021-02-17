
# version 330 core

uniform vec4 u_VecColor = vec4(1.0);
in vec2 TexCoord;
uniform sampler2D texture0;

void main() {
	vec4 tex_color = texture2D(texture0, TexCoord);
	gl_FragColor =  (tex_color + u_VecColor) * 0.5;
}