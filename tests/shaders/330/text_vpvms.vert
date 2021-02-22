# version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
uniform vec3 u_TextPosition = vec3(0);
uniform mat4 u_MatProjection = mat4(1);
uniform mat4 u_MatView = mat4(1);
uniform mat4 u_MatModel = mat4(1);
out vec2 TexCoord;

void main() {
	float depth = (in_Position.x * in_Position.y) / 9999999.0;
	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position.xyz, 1.0);
	gl_Position.z -= depth;
	TexCoord = in_TexCoord;
}