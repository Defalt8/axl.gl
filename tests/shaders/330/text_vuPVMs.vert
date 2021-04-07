# version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_UV;
uniform mat4 u_MatProjection = mat4(1);
uniform mat4 u_MatView = mat4(1);
uniform mat4 u_MatModel = mat4(1);
uniform vec2 u_TextOffset = vec2(0,0);
out vec2 TexCoord;

void main() {
	float depth = (in_Position.x * in_Position.y) / 9999999.0;
	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position.x + u_TextOffset.x, in_Position.y + u_TextOffset.y, depth, 1.0);
	TexCoord = in_UV;
}