#version 410
out vec3 lightOutput;

uniform vec3 lighViewPosition;
uniform vec3 lightDiffuse;
uniform float lightRadius;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main()
{
	//texture coord;
	vec2 texCoord = gl_FragCoord.xy / textureSize(positionTexture, 0).xy;

	//sample textures
	vec3 positionSample = texture(positionTexture, texCoord).xyz;
	vec3 normalSample = texture(normalTexture, texCoord).xyz;
	normalSample *= 2;
	normalSample -= 1;
	//compute lightDirection
	vec3 toLight = lighViewPosition - positionSample;
	vec3 L = normalize(toLight);


	//compute lambertian term
	float d = max(0, dot(normalSample, L));


	//compute fallOff
	float fallOff = 1 - min(1, length(toLight) / lightRadius);


	//outPut lambert * fallOff * colour
	lightOutput = lightDiffuse * d * fallOff;


}