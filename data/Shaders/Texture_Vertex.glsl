#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec2 Tex_coord;
out vec2 frag_tex_coord;
uniform mat4 ProjectionVeiw;
uniform float time;
uniform float heightScale;
void main()
{ 
	frag_tex_coord = Tex_coord;
	gl_Position = ProjectionVeiw * Position;
}