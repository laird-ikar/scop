#version 330 core
out vec4		FragColor;
uniform float	time;

void main() {
	if (gl_FrontFacing)
	{
		vec3 color = vec3(sin(time), cos(time), 0.5 * sin(time + 1.0));
		FragColor = vec4(color, 1.0);
	}
}
