//with assistance from Kyle Mcarthur
#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

out vec2 fragTexCoord;
out vec4 normals;

uniform mat4 viewProj;
uniform mat4 worldProj;
uniform sampler2D perlinTexture;
void main()
{
	int offsetPos = 50;
	fragTexCoord = texCoord;

	vec4 pos = position;
	pos.y += texture(perlinTexture, texCoord).r * 300;
	gl_Position = viewProj * pos;

	float texturesize = textureSize( perlinTexture, 0 );
	float textureoffset = 1 / texturesize;
	float sampleSize = 1.0f;

	vec4 posUp = pos;
	posUp.x += 0;
	posUp.y = texture(perlinTexture, texCoord + vec2( 0, textureoffset).r) * offsetPos;
	posUp.z += sampleSize;
	
	vec4 posUpRight = pos;
	posUpRight.x += sampleSize;
	posUpRight.y = texture(perlinTexture, texCoord + vec2( textureoffset, textureoffset).r) * offsetPos;
	posUpRight.z += sampleSize;

	vec4 posRight = pos;
	posRight.x += sampleSize;
	posRight.y = texture(perlinTexture, texCoord + vec2( textureoffset, 0).r) * offsetPos;

	vec4 posDownRight = pos;
	posDownRight.x += sampleSize;
	posDownRight.y = texture(perlinTexture, texCoord + vec2( textureoffset, -textureoffset ).r) * offsetPos;
	posDownRight.z -= sampleSize;

	vec4 posDown = pos;
	posDown.x += 0;
	posDown.y = texture(perlinTexture, texCoord + vec2( 0, -textureoffset ).r) * offsetPos;
	posDown.z -= sampleSize;

	vec4 posDownLeft = pos;
	posDownLeft.x -= sampleSize;
	posDownLeft.y = texture(perlinTexture, texCoord + vec2( -textureoffset, -textureoffset ).r) * offsetPos;
	posDownLeft.z -= sampleSize;

	vec4 posLeft = pos;
	posLeft.x -= sampleSize;
	posLeft.y = texture(perlinTexture, texCoord + vec2( -textureoffset, 0 ).r) * offsetPos;
	posLeft.z += 0;

	vec4 posUpLeft = pos;
	posUpLeft.x -= sampleSize;
	posUpLeft.y = texture(perlinTexture, texCoord + vec2( -textureoffset, textureoffset ).r) * offsetPos;
	posUpLeft.z += sampleSize;

	vec4 finalNormal = vec4(0,0,0,0);
	//


	finalNormal.xyz += cross( posUp.xyz - pos.xyz, posUpRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posUpRight.xyz - pos.xyz, posRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posRight.xyz - pos.xyz, posDownRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posDownRight.xyz - pos.xyz, posDown.xyz - pos.xyz);

	finalNormal.xyz += cross( posDown.xyz - pos.xyz, posDownLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posDownLeft.xyz - pos.xyz, posLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posLeft.xyz - pos.xyz, posUpLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posUpLeft.xyz - pos.xyz, posUp.xyz - pos.xyz);


	normals = normalize( finalNormal );
}