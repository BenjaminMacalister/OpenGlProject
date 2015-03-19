#version 410

in vec4 fragNormal;
in vec4 shadowCoord;
out vec4 fragColour;

uniform vec3 lighDir;
uniform sampler2D shadowMap;

void main()
{
	float d = max(0, dot(-lighDir, normalize(fragNormal).xyz));
	float shadowMapSample = texture(shadowMap, shadowCoord.xy).r;

	if(shadowMapSample < shadowCoord.z - 0.01f)
	{
		d = 0;
	}

	fragColour = vec4(d,d,d,1);
}