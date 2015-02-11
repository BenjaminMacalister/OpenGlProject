#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
out vec4 vColour;
uniform mat4 ProjectionVeiw;
uniform float time;
uniform float heightScale;
void main() 
{ 
	vec4 nColour;
	nColour.x = 0;
	nColour.y = 0;
	nColour.z = Colour.x +sin(time + Position.x) * heightScale;
	vColour = nColour;
	vec4 P = Position;
	P.y += sin(time + Position.x) * heightScale;
	P.y += sin(time + Position.z) * heightScale;
	gl_Position = ProjectionVeiw * P;
}