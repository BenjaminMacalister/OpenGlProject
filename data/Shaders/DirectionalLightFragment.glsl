#version 410

in vec2 fragTexcoord;
out vec3 outColour;

uniform vec3 LightDir;
uniform vec3 lightColour;

uniform sampler2D positionTex;
uniform sampler2D normalTex;

void main()
{
	vec3 normalSample = texture(normalTex, fragTexcoord).xyz;
	normalSample *= 2;
	normalSample -=1;
	vec3 positionSample = texture(positionTex, fragTexcoord).xyz;

	//vec3 N = normalize(normalSample);

	float d = max(0, dot(-LightDir, normalSample));

	outColour = lightColour *d;
}