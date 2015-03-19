#version 410
layout(location = 0) in vec3 position;

uniform mat4 lighMatrix;

void main()
{
	gl_Position = lighMatrix * vec4(position, 1);
}