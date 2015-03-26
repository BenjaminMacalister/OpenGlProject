#version 410

layout(location=0) in vec4 position;

uniform vec3 lightPosition;
uniform float lightRadius;
uniform mat4 projView;

void main()
{
	gl_Position = projView * vec4(position.xyz * lightRadius + lightPosition, 1);
}