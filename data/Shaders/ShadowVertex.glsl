#version 410

layout(location=0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 fragNormal;
out vec4 shadowCoord;

uniform mat4 lightMatrix;

uniform mat4 viewProj;

void main()
{
	fragNormal = vec4(normal, 0);
	shadowCoord = lightMatrix * vec4(position, 1);
	gl_Position = viewProj * vec4(position, 1);
}