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
	bool StartUp();
	void ShutDown();
	bool Update(float dt);
	void Draw(FlyCamera a_camera);

	void GenerateGLMeshes1(FBXFile* fbx);

	void EvaluateSkeleton1(FBXAnimation* anim, FBXSkeleton* skeleton, float timer);
	void UpdateBones1(FBXSkeleton* skeleton);
	std::vector<openGLData> m_meshes;

	FBXFile* m_file;
	float m_timer;
	unsigned int m_animationID;
};



#endif