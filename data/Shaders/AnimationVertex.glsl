#version 410
layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texcoord;
layout( location = 2 ) in vec4 boneIndices;
layout( location = 3 ) in vec4 boneWeights;
layout( location = 4 ) in vec4 Normal;

out vec2 frag_texcoord;
out vec4 frag_normal;
uniform mat4 projection_view;
uniform mat4 world;
uniform vec4 worldPosition;
// we need to give our bone array a limit
const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];
void main()
{
	frag_texcoord = texcoord;
	ivec4 indices = ivec4(boneIndices);
	vec4 finalPosition = worldPosition;//vec4(30000,16000,-40000, 0);
	finalPosition += bones[indices.x] * position * boneWeights.x*5;
	finalPosition += (bones[indices.y] * position * boneWeights.y*5);
	finalPosition += bones[indices.z] * position * boneWeights.z*5;
	finalPosition += bones[indices.w] * position * boneWeights.w*5;
	finalPosition.w = 1;
	vec4 finalNormal = vec4(0);
	finalNormal += bones[indices.x] * Normal * boneWeights.x*5;
	finalNormal += (bones[indices.y] * Normal * boneWeights.y*5);
	finalNormal += bones[indices.z] * Normal * boneWeights.z*5;
	finalNormal += bones[indices.w] * Normal * boneWeights.w*5;
	finalNormal.w = 1;
	
	frag_normal = finalNormal;
	gl_Position = projection_view * world * finalPosition;
}