#version 330 core

in vec2 uv;
uniform sampler2D textureSampler;

void main() {
	gl_FragColor = texture(textureSampler, uv);
}
