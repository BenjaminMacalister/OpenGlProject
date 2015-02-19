#version 410
in vec4 frag_normal;
in vec4 frag_Position;
out vec4 frag_colour;

uniform vec3 light_dir;
uniform vec3 light_colour;
uniform vec3 material_colour;
uniform vec3 ambient_light;
uniform vec3 eye_pos;
uniform float spec_power;

void main()
{

	vec3 ambient = material_colour * ambient_light;

	vec3 N = normalize(frag_normal.xyz);
	vec3 L = normalize(light_dir);
	
	float d = max(0,dot(N, -L));
	vec3 FinalDiffuse = vec3(d) * light_colour * material_colour;

	vec3 E = normalize(eye_pos - frag_Position.xyz);
	vec3 R = reflect(L,N);

	float s = max(0,dot(R, E));
	s = pow(s, spec_power);
	vec3 specular = vec3(s)*light_colour*material_colour;

	frag_colour = vec4(ambient + FinalDiffuse + specular,1);
}