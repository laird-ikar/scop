#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

uniform float	time;
uniform float	view_depth;

flat out	vec4		color;
out			vec2		uv;

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
	color = vec4(aColor, 1.0);
	uv = aUV;
}
