
# version 330 core

in vec2 TexCoord;
uniform sampler2D texture0;

void main() {
	gl_FragColor = texture2D(texture0, TexCoord);
}