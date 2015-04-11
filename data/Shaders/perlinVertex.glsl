#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat4 viewProj;
uniform sampler2D perlinTexture;
void main()
{
	fragTexCoord = texCoord;

	vec4 pos = position;
	pos.y += texture(perlinTexture, texCoord).r * 300;
	gl_Position = viewProj * pos;
}