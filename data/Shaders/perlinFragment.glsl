#version 410

in vec2 fragTexCoord;

out vec4 fragColour;
uniform sampler2D perlinTexture;
void main()
{

	vec4 tempColour = texture(perlinTexture, fragTexCoord);
	float edge = clamp(tempColour, 0, 1);
	tempColour. a = 1;
	if(tempColour.r < 1 && tempColour.r > 0.5)
	{
		tempColour = mix(vec4(1,0,0,1), vec4(0,1,0,1), 1 - edge);
	}
	if(tempColour.r < 0.5 && tempColour.r > 0.3)
	{
		tempColour = mix(vec4(0,1,0,1), vec4(0,0,1,1), 1 - edge);
	}
	if(tempColour.r < 0.3 && tempColour.r > 0)
	{
		tempColour = mix(vec4(0,0,1,1), vec4(1,1,0,1), 1 - edge);
	}

	//step(tempColour.r, 0.3) * mix(vec4(0,0,1,1), vec4(1,1,0,1), 1 - edge);
	fragColour = tempColour;

}