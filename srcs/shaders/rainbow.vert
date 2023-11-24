#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform float	time;

void main()
{
	mat3 projection;
	mat3 rotation;


	projection[0] = vec3(1.0, 0.0, 0.0);
	projection[1] = vec3(0.0, 1.0, 0.0);
	projection[2] = vec3(0.0, 0.0, 1.0);

	rotation[0] = vec3(cos(time), 0.0, sin(time));
	rotation[1] = vec3(0.0, 1.0, 0.0);
	rotation[2] = vec3(-sin(time), 0.0, cos(time));


	vec3 projected = projection * rotation * aPos;
	gl_Position = vec4(projected.x, projected.y, projected.z, 1.0);
}
