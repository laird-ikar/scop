#version 330 core

in vec2 uv;
uniform sampler2D textureSampler;

flat in vec4 color;
flat in vec4 greyscale;
flat in vec4 normal;

uniform int mode;

void main() {
	if (mode == 0)
	{
		gl_FragColor = color;
	}
	else if (mode == 1)
	{
		gl_FragColor = greyscale;
	}
	else if (mode == 2)
	{
		gl_FragColor = normal;
	}
	else if (mode == 3)
	{
		gl_FragColor = texture(textureSampler, uv);
	}
}
