#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 texcoord;

out vec2 fragTexcoord;

void main()
{
	gl_Position = Position;
	fragTexcoord = texcoord;
}