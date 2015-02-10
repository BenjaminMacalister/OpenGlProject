#ifndef __INTROTOOPENGL_H__
#define __INTROTOOPENGL_H__
#include "Application.h"
#include "CameraClass.h"
class IntroToOpenGl : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();

	mat4 m_projection;
	mat4 m_view;
	float m_camera_x;
	float m_camera_z;
	float m_timer;
	FlyCamera m_Camera;
};

#endif