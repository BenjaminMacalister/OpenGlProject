#version 410

layout(location=0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec4 fragNormal;
out vec4 shadowCoord;

uniform mat4 lightMatrix;

uniform mat4 viewProj;

void main()
{
	fragNormal = vec4(normal.xyz, 0);
	shadowCoord = lightMatrix * position;
	gl_Position = viewProj * vec4(position.xyz, 1);
}