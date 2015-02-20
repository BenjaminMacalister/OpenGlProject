#ifndef _Quat_H_
#define _Quat_H_

#include "Application.h"
#include "CameraClass.h"
struct KeyFrame
{
	vec3 position;
	glm::quat rotation;
};

class Quaternions : public Application
{
public:
	virtual bool StartUp();
	virtual void ShutDown();
	virtual bool Update();
	virtual void Draw();
	FlyCamera m_Camera;

	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];
	KeyFrame m_footFrame[2];

	mat4 m_hip_bone;
	mat4 m_knee_bone;
	mat4 m_ankle_bone;
	mat4 m_foot_bone;

	float m_timer;
};



#endif