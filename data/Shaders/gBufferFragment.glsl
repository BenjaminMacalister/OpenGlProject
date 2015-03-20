#version 410
in vec4 fragPosition;
in vec4 fragNormal;

layout(location = 0) out vec3 gAlbedo;
layout(location = 1) out vec3 gPosition;
layout(location = 2) out vec3 gNormal;

void main()
{
	gPosition = fragPosition.xyz;
	gNormal = (fragNormal.xyz) * 0.5f + 0.5f;

	gAlbedo = vec3(1);
}