#version 410
in vec3 frag_normal;
in vec3 frag_position;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec2 frag_texcoord;

out vec4 frag_colour;

uniform vec3 light_dir;
uniform vec3 light_colour;

uniform vec3 ambient_light;
uniform vec3 eye_pos;
uniform float spec_power;

uniform sampler2D diffuse_tex;
uniform sampler2D normal_tex;
uniform sampler2D specular_tex;

void main()
{
	mat3 TBN = mat3(normalize(frag_tangent), normalize(frag_bitangent), normalize(frag_normal));
	
	vec3 sampled_normal = texture(normal_tex, frag_texcoord).xyz;
	vec3 adjusted_normal = sampled_normal *2 -1;

	vec3 N = normalize(TBN * adjusted_normal);
	vec3 material_colour = texture(diffuse_tex, frag_texcoord).xyz;

	vec3 ambient = material_colour * ambient_light;

	//vec3 N = normalize(frag_normal);
	vec3 L = normalize(light_dir);
	
	float d = max(0,dot(N, -L));
	vec3 FinalDiffuse = vec3(d) * light_colour * material_colour;

	vec3 E = normalize(eye_pos - frag_position);
	vec3 R = reflect(L,N);

	float s = max(0,dot(R, E));
	vec3 material_specular = texture(specular_tex, frag_texcoord).xyz;
	s = pow(s, spec_power);
	vec3 specular = vec3(s)*light_colour*material_specular;

	frag_colour = vec4(ambient + FinalDiffuse + specular,1);
}