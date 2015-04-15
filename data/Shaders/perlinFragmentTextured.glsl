#version 410

in vec2 fragTexCoord;
in vec4 normals;

out vec4 fragColour;
uniform sampler2D perlinTexture;

uniform sampler2D RockTexture;
uniform sampler2D GrassTexture;
uniform sampler2D WaterTexture;
uniform vec3 m_lightDirection;

void main()
{
	vec4 tempColour = texture(perlinTexture, fragTexCoord);
	vec4 tempColour1 = texture(perlinTexture, fragTexCoord);
	vec4 rockColour = texture(RockTexture, fragTexCoord);
	vec4 grassColour = texture(GrassTexture, fragTexCoord);
	vec4 waterColour = texture(WaterTexture, fragTexCoord);
	float edge = clamp(tempColour, 0, 1);


	tempColour1 = rockColour;
	if(tempColour.r <= 1 && tempColour.r > 0.5)
	{

		tempColour1 = rockColour;

	}
	if(tempColour.r < 0.5 && tempColour.r > 0.3)
	{
		tempColour1 = mix(rockColour, grassColour, 1 - edge);
	}
	if(tempColour.r < 0.3 && tempColour.r > 0)
	{
		tempColour1 = mix(grassColour, waterColour, 1 - edge);
	}
	vec3 ambient = tempColour1.xyz * vec3(0.1,0.1,0.1);

	vec3 N = normalize(normals.xyz);

	vec3 L = normalize(m_lightDirection);
	float d = max(0,dot(N, -L));
	vec3 FinalDiffuse = vec3(d) * vec3(1,1,1) * tempColour1.xyz;

	fragColour = vec4(ambient + FinalDiffuse,1);

}