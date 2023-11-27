#version 330 core
layout (location = 0) in vec3 aPos;

uniform float	time;
uniform float	view_depth;

void main()
{
	vec4 homogenous = vec4(aPos / view_depth, view_depth);
	mat4 projection = mat4(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 1,
							0, 0, 0, 1);
	mat4 rotY	 = mat4(	cos(time),	0,	-sin(time),	0,
							0,			1,	0,			0,
							sin(time),	0,	cos(time),	0,
							0,			0,	0,			1);
	gl_Position = projection * rotY * homogenous;
}
