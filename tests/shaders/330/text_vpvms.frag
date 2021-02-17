
# version 330 core

in vec2 TexCoord;
uniform vec4 u_TextColor = vec4(1.0,1.0,1.0,1.0);
uniform sampler2D texture0;

void main() {
	vec4 sample = texture2D(texture0, TexCoord);
	gl_FragColor = vec4(u_TextColor * sample.r);
}