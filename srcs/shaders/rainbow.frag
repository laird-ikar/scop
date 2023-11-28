#version 330 core

out vec4		FragColor;
uniform float	time;

flat in	vec4		color;

void main() {
	FragColor = color;
	// FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
