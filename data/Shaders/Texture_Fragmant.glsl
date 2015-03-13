#version 410

//in vec2 frag_tex_coord;
in vec4 reflectedScreenPos;
out vec4 frag_colour;

uniform sampler2D diffuse;
uniform float time;

void main()
{
	//frag_colour = sin((2+time)* texture(diffuse, frag_tex_coord*(9+time)*2));





	vec4 uvPosition = reflectedScreenPos / reflectedScreenPos.w;
	uvPosition = (uvPosition + 1) * 0.5f;
	frag_colour = texture(diffuse,uvPosition.xy);
}