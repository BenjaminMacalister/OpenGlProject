#version 410

in vec2 frag_texcoord;
in vec4 frag_normal;
out vec4 frag_colour;

uniform sampler2D diffuse;
uniform vec3 light_dir;
uniform vec3 light_colour;


void main()
{

	vec4 m_colour = texture(diffuse, frag_texcoord);
	vec4 AmbiantColour = m_colour * vec4(0.1,0.1,0.1,1);
	vec3 N = normalize(frag_normal.xyz);
	vec3 L = normalize(vec3(light_dir));//(-3,-2.5,0));

	float d = max(0,dot(N, -L));
	vec3 FinalDiffuse = vec3(d) * vec3(1,1,1) * m_colour.xyz;
	frag_colour = vec4(AmbiantColour.xyz + FinalDiffuse,1);
}