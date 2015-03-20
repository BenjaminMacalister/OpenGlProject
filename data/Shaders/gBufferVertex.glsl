#version 410
layout(location=0) in vec4 Position;
layout(location = 1) in vec4 Normal;

out vec4 fragPosition;
out vec4 fragNormal;

uniform mat4 View;
uniform mat4 ProjectionView;

void main()
{
	fragPosition = View * vec4(Position.xyz, 1);
	fragNormal = View * vec4(Normal.xyz, 0);

	gl_Position = ProjectionView * Position;
}