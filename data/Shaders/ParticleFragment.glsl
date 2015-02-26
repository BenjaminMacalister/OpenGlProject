#version 410
in vec4 vColour;
out vec4 fragColour;
uniform sampler2D diffuse_tex;
in vec2 frag_texcoord;
void main()
{
	vec4 material_colour = texture(diffuse_tex, frag_texcoord);
	fragColour = vec4(material_colour);
}