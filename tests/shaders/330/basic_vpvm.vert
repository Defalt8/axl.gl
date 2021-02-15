# version 330 core

layout(location = 0) in vec3 in_Position;
uniform mat4 u_MatProjection = mat4(1);
uniform mat4 u_MatView = mat4(1);
uniform mat4 u_MatModel = mat4(1);

void main() {
	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position.xyz, 1.0);
}