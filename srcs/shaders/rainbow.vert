#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	normal = vec3(aNormal.x, aNormal.y, aNormal.z);
}
