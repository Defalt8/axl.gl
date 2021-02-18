# version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_UV;
uniform mat4 u_MatProjection = mat4(1);
uniform mat4 u_MatView = mat4(1);
uniform mat4 u_MatModel = mat4(1);
out vec2 TexCoord;

void main() {
	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position.xy, 0.0, 1.0);
	TexCoord = in_UV;
}