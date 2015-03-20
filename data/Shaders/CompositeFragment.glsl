#version 410

in vec2 fragTexcoord;

out vec4 outColour;


uniform sampler2D albedoTex;
uniform sampler2D lightTex;
void main()
{
	vec4 albidoSample = texture(albedoTex, fragTexcoord);
	vec4 lightSample = texture(lightTex, fragTexcoord);
	outColour = albidoSample * lightSample;
}