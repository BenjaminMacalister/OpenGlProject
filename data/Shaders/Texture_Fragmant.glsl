#version 410

in vec2 frag_tex_coord;
out vec4 frag_colour;

uniform sampler2D diffuse;
uniform float time;

void main()
{
	frag_colour = sin((2+time)* texture(diffuse, frag_tex_coord*(9+time)*2));
}