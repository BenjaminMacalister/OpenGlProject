#ifndef __CameraClass__H__
#define __CameraClass__H__
#include "Application.h"

class Camera
{
public:
	Camera();
	~Camera();
	mat4 m_WorldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	virtual bool update();

	virtual void SetPerspective(float a_FOV, float a_AR, float a_N, float a_f);

	virtual void setLookAt(vec3 from, vec3 to, vec3 up);
	virtual void setPosition(vec3 position);
	virtual mat4 getWorldTransform();
	virtual mat4 getView();
	virtual mat4 getProjection();
	virtual mat4 getProjectionView();
	virtual bool updateProjectionViewTransform();

};

class FlyCamera : public Camera
{
private:
	float m_speed;

public:
	virtual bool update(float a_DT);
	virtual void SetSpeed(float a_speed);

};



#endif