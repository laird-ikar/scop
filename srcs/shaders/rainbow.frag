#version 330 core
out vec4		FragColor;
uniform float	time;

void main() {
	FragColor = vec4(1-gl_FragCoord.z, 1-gl_FragCoord.z, 1-gl_FragCoord.z, 1.0);
	// FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
