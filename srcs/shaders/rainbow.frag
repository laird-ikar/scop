#version 330 core
out vec4		FragColor;
uniform float	time;

in vec3 normal;

void main() {
	if (gl_FragCoord.z < 0.5)
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		FragColor = vec4(0.0, 1.0, 1.0, 1.0);

}
