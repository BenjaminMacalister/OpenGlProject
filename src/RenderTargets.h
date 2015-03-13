#ifndef _REDERTARG_H_
#define _REDERTARG_H_

#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"
class RenderTarget : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();

	void GenerateFrameBuffer();
	void GeneratePlane();
	FlyCamera m_Camera;
	Camera m_mirrorCamera;

	unsigned int m_FBO;
	unsigned int m_FBOTexture;
	unsigned int m_FBODepth;
	unsigned int m_programID;
	openGLData m_plane;
};



#endif