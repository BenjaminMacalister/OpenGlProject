#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Application.h"
#include "CameraClass.h"
#include "FBXFile.h"
#include "Vertex.h"
#include "Utility.h"

class Animation : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();

	void GenerateGLMeshes(FBXFile* fbx);

	void EvaluateSkeleton(FBXAnimation* anim, FBXSkeleton* skeleton, float timer);
	void UpdateBones(FBXSkeleton* skeleton);
	std::vector<openGLData> m_meshes;

	FlyCamera m_Camera;
	FBXFile* m_file;
	float m_timer;
	unsigned int m_ProgramID;
};



#endif