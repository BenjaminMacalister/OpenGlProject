#ifndef _AssignmentANIMATION_H_
#define _AssignmentANIMATION_H_

#include "Application.h"
#include "CameraClass.h"
#include "FBXFile.h"
#include "Vertex.h"
#include "Utility.h"

class Animation
{
public:
	bool StartUp(char* CharacterFile, vec2 minMaxTime);
	void ShutDown();
	bool Update(float dt);
	void Draw(FlyCamera a_camera, vec3 ligthDirection, vec3 positions);

	void GenerateGLMeshes1(FBXFile* fbx);

	void EvaluateSkeleton1(FBXAnimation* anim, FBXSkeleton* skeleton, float timer);
	void UpdateBones1(FBXSkeleton* skeleton);
	std::vector<openGLData> m_meshes;

	FBXFile* m_file;
	FBXFile* m_file2;
	float m_timer;
	float m_minTimer, m_maxTimer;
	unsigned int m_animationID;
};



#endif