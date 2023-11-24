#version 330 core
layout (location = 0) in vec3 aPos;

uniform float	time;

void main()
{
	mat3 projection;
	mat3 rotationY;

	projection[0] = vec3(1.0, 0.0, 0.0);
	projection[1] = vec3(0.0, 1.0, 0.0);
	projection[2] = vec3(0.0, 0.0, 1.0);

	rotationY[0] = vec3(cos(time), 0.0, sin(time));
	rotationY[1] = vec3(0.0, 1.0, 0.0);
	rotationY[2] = vec3(sin(time), 0.0, cos(time));

	vec3 projected =
		projection
		* rotationY
		* aPos;
	gl_Position = vec4(projected.x, projected.y, projected.z, 1.0);
}
