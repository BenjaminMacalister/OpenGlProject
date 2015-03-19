#ifndef _PostPro_H_
#define _PostPro_H_

#include "Application.h"
#include "CameraClass.h"
#include "Vertex.h"
class PostProcess : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();

	void generateFrameBuffer();
	void generateScreenSpaceQuad();
	unsigned int m_FBO;
	unsigned int m_FBOTexture;
	unsigned int m_FBODepth;
	unsigned int m_postProgramID;
	float m_time;
	openGLData m_quad;

	FlyCamera m_Camera;
};



#endif