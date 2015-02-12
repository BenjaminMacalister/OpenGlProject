#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "Application.h"
#include "CameraClass.h"

class FBXFile;

class Lighting : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	unsigned int m_ProgramID;
	void CreateOpenGLBuffers(FBXFile* file);
	void CleanUpOpenGLBuffers(FBXFile* file);
	void reloadShader();
	FBXFile* m_FBXFile;
	FlyCamera m_Camera;
};



#endif