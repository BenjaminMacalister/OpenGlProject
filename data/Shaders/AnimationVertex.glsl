#version 410
layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texcoord;
layout( location = 2 ) in vec4 boneIndices;
layout( location = 3 ) in vec4 boneWeights;

out vec2 frag_texcoord;
uniform mat4 projection_view;
uniform mat4 world;
// we need to give our bone array a limit
const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];
void main()
{
	frag_texcoord = texcoord;

	ivec4 indices = ivec4(boneIndices);
	vec4 finalPosition = vec4(0,0,0,0);
	finalPosition += bones[indices.x] * position * boneWeights.x;
	finalPosition += bones[indices.y] * position * boneWeights.y;
	finalPosition += bones[indices.z] * position * boneWeights.z;
	finalPosition += bones[indices.w] * position * boneWeights.w;
	finalPosition.w = 1;
	gl_Position = projection_view * world * finalPosition;
}