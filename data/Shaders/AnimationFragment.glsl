#version 410

in vec2 frag_texcoord;
out vec4 frag_colour;

uniform sampler2D diffuse;

void main()
{
	frag_colour = texture(diffuse, frag_texcoord);
}