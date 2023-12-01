#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform float	time;
uniform float	view_depth;
uniform float	deltaX;
uniform float	deltaY;
uniform float	rotationZ;
uniform float	rotationX;

flat out	vec4		color;
flat out	vec4		greyscale;
flat out	vec4		normal;
out			vec2		uv;

void main()
{
	vec4 homogenous = vec4(aPos / view_depth, view_depth);
	mat4 projection = mat4(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 1,
							deltaX, deltaY, 0, 1);
	mat4 rotY	 = mat4(	cos(time),	0,	-sin(time),	0,
							0,			1,	0,			0,
							sin(time),	0,	cos(time),	0,
							0,			0,	0,			1);
	mat4 rotZ	 = mat4(	cos(rotationZ),	-sin(rotationZ),	0,	0,
							sin(rotationZ),	cos(rotationZ),		0,	0,
							0,				0,					1,	0,
							0,				0,					0,	1);
	mat4 rotX	 = mat4(	1,	0,				0,					0,
							0,	cos(rotationX),	-sin(rotationX),	0,
							0,	sin(rotationX),	cos(rotationX),		0,
							0,	0,				0,					1);
	gl_Position = projection * rotY * rotZ * rotX * homogenous;
	color = vec4(aColor, 1.0);
	greyscale = vec4(aColor.r, aColor.r, aColor.r, 1.0);
	normal = vec4(aNormal / 2 + 0.5, 1.0);
	uv = aUV;
}
