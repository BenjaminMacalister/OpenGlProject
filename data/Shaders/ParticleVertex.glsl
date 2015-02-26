#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
layout(location=2) in vec2 tex_coord;
out vec4 vColour;
out vec2 frag_texcoord;
uniform mat4 ProjectionVeiw;
void main()
{ 
	frag_texcoord = tex_coord;
	vColour = Colour;

	gl_Position = ProjectionVeiw * Position;
}