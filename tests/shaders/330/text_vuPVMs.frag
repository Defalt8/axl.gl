
# version 330 core

in vec2 TexCoord;
uniform vec4 u_TextColor = vec4(0.5,0.5,0.5,1.0);
uniform sampler2D texture;

void main() {
	vec4 sample = texture2D(texture, TexCoord);
	gl_FragColor = vec4(u_TextColor.rgb, u_TextColor.a * sample.r);
}