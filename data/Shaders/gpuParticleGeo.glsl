#version 410
layout(points) in;
layout(triangle_strip, max_vertices =4) out;

in vec3 geoPosition[];
in float geoLifetime[];
in float geoLifespan[];

out vec4 fragColour;

uniform mat4 projectionView;
uniform mat4 cameraWorld;

uniform float startSize;
uniform float endSize;

uniform vec4 startColour;
uniform vec4 endColour;

void main()
{
	float t = geoLifetime[0] / geoLifespan[0];
	fragColour = mix(startColour, endColour, t);

	float halfSize = mix(startSize, endSize, t) *0.5;

	vec3 corners[4];

	corners[0] = vec3(halfSize, -halfSize, 0);
	corners[1] = vec3(halfSize, halfSize, 0);
	corners[2] = vec3(-halfSize, -halfSize, 0);
	corners[3] = vec3(-halfSize, halfSize, 0);

	vec3 forward = normalize(cameraWorld[3].xyz - geoPosition[0]);
	vec3 right = cross(cameraWorld[1].xyz, forward);
	vec3 up = cross(forward, right);
	
	mat3 billboardRot = mat3(right, up, forward);
	corners[0] = billboardRot * corners[0];
	corners[1] = billboardRot * corners[1];
	corners[2] = billboardRot * corners[2];
	corners[3] = billboardRot * corners[3];

	corners[0] += geoPosition[0];
	corners[1] += geoPosition[0];
	corners[2] += geoPosition[0];
	corners[3] += geoPosition[0];

	gl_Position = projectionView * vec4(corners[0], 1);
	EmitVertex();
	gl_Position = projectionView * vec4(corners[1], 1);
	EmitVertex();
	gl_Position = projectionView * vec4(corners[2], 1);
	EmitVertex();
	gl_Position = projectionView * vec4(corners[3], 1);
	EmitVertex();
}