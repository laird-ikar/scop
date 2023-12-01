#version 330 core

in vec2 uv;
uniform sampler2D textureSampler;

flat in vec4 color;
flat in vec4 greyscale;
flat in vec4 normal;

void main() {
	gl_FragColor = texture(textureSampler, uv);
	// gl_FragColor = normal;
}
